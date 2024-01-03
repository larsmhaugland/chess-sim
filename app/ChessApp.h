#ifndef EXAMAPPLICATION_H
#define EXAMAPPLICATION_H

#include <string>
#include "GLFWApplication.h"
#include "PerspectiveCamera.h"
#include "Shader.h"
#include "ChessEngine.h"


class ChessApp : public GLFWApplication
{
public:
    ChessApp();
    ChessApp(const std::string& name, const std::string& version);
    ~ChessApp();

    //Initialization
    virtual unsigned int Init();
    // Run function
    virtual unsigned int Run() const override;
    void static keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

protected:
    static PerspectiveCamera* camera;
    static glm::vec2 playerPos;
    static bool texture;
    static float ambientLight;
    static float specStrength;
    static float diffStrength;
    static Shader* cubeShader;
    static Shader* gridShader;
    static ChessEngine* chessEngine;
};

#endif