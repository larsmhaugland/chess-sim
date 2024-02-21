#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>
#include <algorithm>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <thread>
#include <bitset>
#include "ChessApp.h"
#include "Shader.h"
#include "GeometricTools.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "BufferLayout.h"
#include "VertexArray.h"
#include "RenderCommands.h"
#include "PerspectiveCamera.h"
#include "shaders.h"
#include "OrthographicCamera.h"
#include "TextureManager.h"
#include "iomanip"
#include "ChessEngine.h"
#include "ChessPiece.h"
#include "tiny_obj_loader.h"

struct Vertex
{
    glm::vec3 location;
    glm::vec3 normals;
    glm::vec2 texCoords;
};


std::vector<glm::vec3> gridPos;     //Grid coordinates
int selected = 0;                   //Index of selected square
int pos = 0;                        //Position of player in grid

bool dayNightCycle = false;          // if "sun" should move or stand still
bool holdKey = true;               // if key press should only be registered once per press or while key is pressed
bool hiRes = true;                  // if high resolution models should be used or not

int X = 8;	                        //X size of board
int Y = 8;	                        //Y size of board

// Initialize static members
PerspectiveCamera* ChessApp::camera = nullptr;
glm::vec2 ChessApp::playerPos = glm::vec2(0, 0);
bool ChessApp::texture = true;  // if texture should be used or not
float ChessApp::ambientLight = 0.3;
float ChessApp::specStrength = 0.5;
float ChessApp::diffStrength = 1.0;
Shader* ChessApp::cubeShader = nullptr;
Shader* ChessApp::gridShader = nullptr;
ChessEngine* ChessApp::chessEngine = new ChessEngine();


// -----------------------------------------------------------------------------
//  MESSAGE CALLBACK
// -----------------------------------------------------------------------------
void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam)
{
    // Outputs debugging messages from OpenGL
    std::cerr << "GL CALLBACK: "
              << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
              << " type = 0x" << type
              << ", severity = 0x" << severity
              << ", message = " << message << "\n";
}


GLuint LoadModel(std::string path, std::string filename, int& size);

/**
 * Constructor
 *
 * @param name
 * @param version
 */
ChessApp::ChessApp(const std::string& name, const std::string& version) : GLFWApplication(name, version) {
	std::cout << "\tName: " << name << "\n\tVersion: " << version << std::endl;
}
/**
 * Destructor
 */
ChessApp::~ChessApp() = default;


/**
 * Callback function for key presses
 * @param window - window that received the event
 * @param key - key that was pressed
 * @param scancode - system specific scancode of the key
 * @param action - action (press, release, repeat)
 * @param mods - bit field describing which modifier keys were held down
 */
void ChessApp::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    //Only act if key is pressed
    if(action == GLFW_REPEAT && !holdKey){
        return;
    }
    if(action != GLFW_REPEAT && action != GLFW_PRESS){
        return;
    }

    float angle = 0.05f;        // Rotation speed
    float zoomSpeed = 0.05f;    // Zoom speed

    switch(key){
        // Rotate camera:
        case GLFW_KEY_D: camera->RotateCamera(angle); break;
        case GLFW_KEY_A: camera->RotateCamera(-angle); break;
        // Zoom camera:
        case GLFW_KEY_W: camera->ZoomCamera(zoomSpeed); break;
        case GLFW_KEY_S: camera->ZoomCamera(-zoomSpeed); break;
        // Move selector:
        case GLFW_KEY_UP:    playerPos.x += (playerPos.x < X-1) ? 1 : 0; break;
        case GLFW_KEY_DOWN:  playerPos.x -= (playerPos.x > 0)   ? 1 : 0; break;
        case GLFW_KEY_RIGHT: playerPos.y += (playerPos.y < Y-1) ? 1 : 0; break;
        case GLFW_KEY_LEFT:  playerPos.y -= (playerPos.y > 0)   ? 1 : 0; break;
        // Toggle texture:
        case GLFW_KEY_T: texture = !texture; break;
        case GLFW_KEY_H: hiRes = !hiRes; printf("%s\n",hiRes ? "Hi" : "Low"); break;
        // Reset board:
        case GLFW_KEY_R: chessEngine->resetBoard(); playerPos = glm::vec2(0); break;
        // Stop/Resume day night cycle (NB: Does not reset cycle, it continues, only change is if "sun" position updates)
        case GLFW_KEY_SPACE:
            chessEngine->tryMove(playerPos.x * 8 + playerPos.y);
            break;
        case GLFW_KEY_ENTER: holdKey = !holdKey; break;
        default: break;
    }
}

/**
 * Initialization
 * @return 0 if successful
 */
unsigned int ChessApp::Init() {
    width = 1024;
    height = 1024;
    GLFWApplication::Init();
    return 0;
}
/**
 * Run function
 * @return 0 if successful
 */
unsigned int ChessApp::Run() const{
    if (!window) {
        fprintf(stderr, "Failed to open window\n");
        return -1;
    }
    const std::string TEXTURES_DIR = "resources/textures/";

    bool running = true;

    //Enable detailed messages for debug and depth test
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEPTH_TEST);
    glDebugMessageCallback(MessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // To easily convert RGB(255) values to span 0-1:
    float base = 1.0/255.0;
    glm::vec4 blackCol = glm::vec4(base*15, base*7, base*2, 1.0f);
    glm::vec4 whiteCol = glm::vec4(base*210, base*180, base*140, 1.0f);


    //Load chess piece models
    int pawnSize = 0;
    auto pawnVAO = LoadModel("resources/models/", "Pawn_hi", pawnSize);
    int rookSize = 0;
    auto rookVAO = LoadModel("resources/models/", "Rook_hi", rookSize);
    int knightSize = 0;
    auto knightVAO = LoadModel("resources/models/", "Knight_hi", knightSize);
    int bishopSize = 0;
    auto bishopVAO = LoadModel("resources/models/", "Bishop_hi", bishopSize);
    int queenSize = 0;
    auto queenVAO = LoadModel("resources/models/", "Queen_hi", queenSize);
    int kingSize = 0;
    auto kingVAO = LoadModel("resources/models/", "King_hi", kingSize);



    // Create grid and cube geometry
    auto gridVertices = GeometricTools::UnitGridGeometry2D(X, Y);
    auto gridIndices = GeometricTools::GridIndices(X, Y);

    auto cubeVertices = GeometricTools::Cube3DWNormals(X);
    auto cubeIndices = GeometricTools::cubeTopologyWNormals;

    // Grid buffers and vertex array
    std::shared_ptr<VertexBuffer> gridVB = std::make_shared<VertexBuffer>(gridVertices.data(), gridVertices.size() * sizeof(gridVertices[0]));
    std::shared_ptr<IndexBuffer> gridIB = std::make_shared<IndexBuffer>(gridIndices.data(), gridIndices.size());
    std::shared_ptr<VertexArray> gridVA = std::make_shared<VertexArray>();
    // Cube buffers and vertex array
    std::shared_ptr<VertexBuffer> cubeVB = std::make_shared<VertexBuffer>(cubeVertices.data(), cubeVertices.size() * sizeof(cubeVertices[0]));
    std::shared_ptr<IndexBuffer> cubeIB = std::make_shared<IndexBuffer>(cubeIndices.data(), cubeIndices.size());
    std::shared_ptr<VertexArray> cubeVA = std::make_shared<VertexArray>();

    // Set buffer layouts
    auto gridBufferLayout = BufferLayout({
                                                 {ShaderDataType::Float2, "position"}
                                         });
    auto cubeBufferLayout = BufferLayout({
                                                 {ShaderDataType::Float3, "position"}, {ShaderDataType::Float3, "normals"}
                                         });
    // Bind buffers to vertex array
    gridVA->Bind();
    gridVB->SetLayout(gridBufferLayout);
    gridVA->AddVertexBuffer(gridVB);
    gridVA->SetIndexBuffer(gridIB);
    gridVA->Unbind();

    // Bind buffers to vertex array
    cubeVA->Bind();
    cubeVB->SetLayout(cubeBufferLayout);
    cubeVA->AddVertexBuffer(cubeVB);
    cubeVA->SetIndexBuffer(cubeIB);
    cubeVA->Unbind();

    //Create grid positions

    float squareSize = 1.0f / X;
    float offset = squareSize * (X / 2 - 1);
    //Create grid coordinates with all squares
    for(int i = 0; i < X; i++){
        for(int j = 0; j < Y; j++){
            float x = (i - 0.5f) * squareSize - offset; // calculate x position of square
            float y = 0.02f;
            float z = (j - 0.5f) * squareSize - offset; // calculate z position of square
            gridPos.emplace_back(x,y,z);
        }
    }

    //Initialize camera
    camera = new PerspectiveCamera();
    camera->SetLookAt(glm::vec3(0.0f));
    camera->SetPosition(glm::vec3(-1.5f, 1.5f, 0.0f));

    //Load shaders
    gridShader = new Shader(gridVertexShader, gridFragmentShader);
    cubeShader = new Shader(cubeVertexShader, cubeFragmentShader);
    gridShader->use();

    //Set background color
    glm::vec4 bColor = glm::vec4(0.2f,0.2f,0.2f,1.0f);
    RenderCommands::SetClearColor(bColor);

    //Load textures
    TextureManager* textureManager = TextureManager::GetInstance();
    textureManager->LoadTexture2DRGBA("floor_dark", std::string(TEXTURES_DIR) + std::string("dark_wood.png"),0);
    textureManager->LoadTexture2DRGBA("floor_light", std::string(TEXTURES_DIR) + std::string("light_wood.png"),1);
    textureManager->LoadCubeMapRGBA("dark_cubemap", std::string(TEXTURES_DIR) + std::string("dark_wood.png"),2);
    textureManager->LoadCubeMapRGBA("light_cubemap", std::string(TEXTURES_DIR) + std::string("light_wood.png"),3);

    float startTime = glfwGetTime();
    float elapsedTime;
    float cycleDuration = 10.0; // Duration of the day/night cycle in seconds (day and night last 5 seconds each)

    glm::vec3 lightPosition; // Position of the light source
    glm::vec3 lightColor = glm::vec3(1,1,1); // White light
    float lightIntensity; // Intensity of the light source

    //TODO DEBUG ONLY REMOVE LATER
    unsigned int highOld;
    unsigned int lowOld;


    // Set the key callback function
    glfwSetKeyCallback(window, keyCallback);
    do
    {
        RenderCommands::Clear();
        // Get the current time
        float currentTime = glfwGetTime();
        // Calculate the elapsed time in the cycle
        elapsedTime = fmod(currentTime - startTime, cycleDuration);

        // Update light pos
        const float rotationSpeed = glm::two_pi<float>() / cycleDuration;  // Rotation speed in radians per second
        glm::vec3 coordinate(0.0f, 0.8f, 0.0f);  // Initial coordinate of the light source
        // Calculate the rotation angle based on elapsed time
        auto rAngle = elapsedTime * rotationSpeed;
        // Apply rotation only to Y and Z coordinates
        glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), rAngle, glm::vec3(1.0f, 0.0f, 0.0f));
        // Apply rotation transformation to the coordinate
        glm::vec4 rotatedCoordinate = rMat * glm::vec4(coordinate, 1.0f);

        if(dayNightCycle) {
            // Update Y and Z coordinates
            coordinate.y = rotatedCoordinate.y;
            coordinate.z = rotatedCoordinate.z;
        }

        lightPosition = coordinate;
        //Set ambient light and "sunlight" intensity based on height of "sun"
        ambientLight = glm::clamp(coordinate.y, 0.2f, 0.3f); // Can't be pitch black or too bright
        lightIntensity = glm::clamp(coordinate.y,0.0f,1.0f); // No light when sun is down

        // Define the desired position of the floor
        glm::vec3 gridPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        // Define the rotation angle (in degrees)
        float rotationAngle = 0.0f;
        // Create the translation matrix
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), gridPosition);
        // Create the rotation matrix
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        float scaleFactor = 1;
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(scaleFactor));
        // Combine the translation and rotation
        glm::mat4 model = translationMatrix * rotationMatrix * scaleMatrix;

        unsigned int highlightedTilesHigh = 0;
        unsigned int highlightedTilesLow = 0;



        int selectorIndex = playerPos.x + playerPos.y * 8;
        if(selectorIndex > 31){
            highlightedTilesHigh = 1 << selectorIndex;
        } else {
            highlightedTilesLow = 1 << selectorIndex;
        }

        //Print debug info if something changes
        if(highOld != highlightedTilesHigh){
            std::cout << "High: " << std::bitset<32>(highlightedTilesHigh) << std::endl;
        }
        if(lowOld != highlightedTilesLow){
            std::cout << "Low: " << std::bitset<32>(highlightedTilesLow) << std::endl;
        }

        highOld = highlightedTilesHigh;
        lowOld = highlightedTilesLow;

        //Send uniforms to grid shader
        gridShader->use();
        gridShader->setMat4("u_viewMat", camera->GetViewMatrix());
        gridShader->setMat4("u_projMat", camera->GetProjectionMatrix());
        gridShader->setMat4("u_modMat", model);
        gridShader->setInt("u_texture", texture ? 1 : 0); // Convert bool to int (not strictly needed since true == 1 && false == 0)
        gridShader->setFloat("u_ambientStrength", ambientLight);
        gridShader->setUniform3f("u_lightSourcePosition", lightPosition);
        gridShader->setFloat("u_diffuseStrength", diffStrength * lightIntensity);
        gridShader->setFloat("u_specularStrength", specStrength * lightIntensity);
        gridShader->setUniform3f("u_cameraPosition", camera->GetPosition());
        gridShader->setUniform3f("u_normals", glm::vec3(0, 1, 0));
        gridShader->setUniform3f("u_lightColor", lightColor);
        //gridShader->setUInt("u_highlightedHigh", highlightedTilesHigh);
        //gridShader->setUInt("u_highlightedLow", highlightedTilesLow);
        // Draw the grid
        gridVA->Bind();
        RenderCommands::DrawIndex(gridVA, GL_TRIANGLES);
        gridVA->Unbind();

        // Enable cube shader
        cubeShader->use();

        // Translate position of cube
        translationMatrix = glm::translate(glm::mat4(1.0f), lightPosition);
        // Rotate cube
        rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        // Scale cube
        scaleFactor = 0.4;
        scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));
        // Combine the translation, rotation and scale
        model = translationMatrix * rotationMatrix * scaleMatrix;
        // Send uniforms to cube shader
        cubeShader->setMat4("u_model", model);
        cubeShader->setUniform4f("u_cubeColor", glm::vec4(glm::vec3(1, 1, 0), lightIntensity));
        cubeShader->setMat4("u_viewProjMat", camera->GetViewProjectionMatrix());
        cubeShader->setFloat("u_ambientStrength", 1); // Always full intensity since it is a light source
        cubeShader->setUniform3f("u_lightSourcePosition", lightPosition);
        cubeShader->setFloat("u_diffuseStrength", diffStrength * lightIntensity); // multiply by light intensity so that there is no light when the sun is down
        cubeShader->setFloat("u_specularStrength", specStrength * lightIntensity); // multiply by light intensity so that there is no light when the sun is down
        cubeShader->setUniform3f("u_cameraPosition", camera->GetPosition());
        cubeShader->setUniform3f("u_lightColor", lightColor);
        cubeShader->setInt("u_texture", 0); //No texture for light
        cubeShader->setInt("u_shine", 1);
        // Draw cube
        cubeVA->Bind();
        RenderCommands::DrawIndex(cubeVA, GL_TRIANGLES);
        cubeVA->Unbind();

        // Set the color for chess pieces
        cubeShader->setFloat("u_ambientStrength", ambientLight); // Based on sun height
        cubeShader->setInt("u_texture", texture ? 1 : 0);       // Convert bool to int (not strictly needed)
        cubeShader->setInt("u_shine", 5);
        //Draw walls
        for (const auto &piece: chessEngine->getPieces()) {
            cubeShader->setUniform4f("u_cubeColor", piece->isWhite() ? whiteCol : blackCol);
            cubeShader->setBool("u_whitePiece", piece->isWhite());

            if(piece == chessEngine->getSelectedPiece()){
                cubeShader->setUniform4f("u_cubeColor", glm::vec4(0.2,0.7,0.8,1));
            }
            // Translate position of piece
            translationMatrix = glm::translate(glm::mat4(1.0f), gridPos[piece->getPos()]);
            rotationAngle = piece->isWhite() ? 90.0f : -90.0f;
            // Rotate piece
            rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(0,1,0));
            // Scale piece
            scaleFactor = 0.0075;
            scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));
            // Combine the translation, rotation and scale
            model = translationMatrix * rotationMatrix * scaleMatrix;

            // Send model matrix to cube shader
            cubeShader->setMat4("u_model", model);
            switch(piece->getType()){
                case PAWN:
                    glBindVertexArray(pawnVAO);
                    glDrawArrays(GL_TRIANGLES, 0, pawnSize);
                    break;
                case ROOK:
                    glBindVertexArray(rookVAO);
                    glDrawArrays(GL_TRIANGLES, 0, rookSize);
                    break;
                case KNIGHT:
                    glBindVertexArray(knightVAO);
                    glDrawArrays(GL_TRIANGLES, 0, knightSize);
                    break;
                case BISHOP:
                    glBindVertexArray(bishopVAO);
                    glDrawArrays(GL_TRIANGLES, 0, bishopSize);
                    break;
                case QUEEN:
                    glBindVertexArray(queenVAO);
                    glDrawArrays(GL_TRIANGLES, 0, queenSize);
                    break;
                case KING:
                    glBindVertexArray(kingVAO);
                    glDrawArrays(GL_TRIANGLES, 0, kingSize);
                    break;
                default:
                    break;
            }
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        running &= (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE);
        running &= (glfwWindowShouldClose(window) != GL_TRUE);
    } while (running);
    //Free memory and call destructors that need GLFW
    delete camera;
    delete cubeShader;
    delete gridShader;

    //Resetting shared_ptr so that destructor is called before GLFW terminates
    cubeVA.reset();
    cubeVB.reset();
    cubeIB.reset();

    gridVA.reset();
    gridVB.reset();
    gridIB.reset();

    //Close window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

GLuint LoadModel(const std::string path, const std::string filename, int& size)
{
    //We create a vector of Vertex structs. OpenGL can understand these, and so will accept them as input.
    std::vector<Vertex> vertices;

    //Some variables that we are going to use to store data from tinyObj
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials; //This one goes unused for now, seeing as we don't need materials for this model.

    //Some variables incase there is something wrong with our obj file
    std::string warn;
    std::string err;


    //We use tinobj to load our models. Feel free to find other .obj files and see if you can load them.
    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (path + filename + ".obj").c_str(), path.c_str());

    if (!warn.empty()) {
        std::cout << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    //For each shape defined in the obj file
    for (const auto& shape : shapes)
    {
        //We find each mesh
        for (auto meshIndex : shape.mesh.indices)
        {
            //And store the data for each vertice, including normals
            glm::vec3 vertice = {
                    attrib.vertices[meshIndex.vertex_index * 3],
                    attrib.vertices[(meshIndex.vertex_index * 3) + 1],
                    attrib.vertices[(meshIndex.vertex_index * 3) + 2]
            };
            glm::vec3 normal = {
                    attrib.normals[meshIndex.normal_index * 3],
                    attrib.normals[(meshIndex.normal_index * 3) + 1],
                    attrib.normals[(meshIndex.normal_index * 3) + 2]
            };
            glm::vec2 textureCoordinate = {                         //These go unnused, but if you want textures, you will need them.
                    attrib.texcoords[meshIndex.texcoord_index * 2],
                    attrib.texcoords[(meshIndex.texcoord_index * 2) + 1]
            };

            vertices.push_back({ vertice, normal, textureCoordinate }); //We add our new vertice struct to our vector

        }
    }

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //As you can see, OpenGL will accept a vector of structs as a valid input here
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

    //This will be needed later to specify how much we need to draw. Look at the main loop to find this variable again.
    size = vertices.size();

    return VAO;
}