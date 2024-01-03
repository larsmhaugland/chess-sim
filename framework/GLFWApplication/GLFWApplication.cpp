#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cstring>
#include "GLFWApplication.h"
#include <iostream>



GLFWApplication::GLFWApplication(const std::string& name, const std::string& version) {this->name = name;}
GLFWApplication::~GLFWApplication() {}


void GLFWErrorCallback(int code, const char* description)
{
    std::cerr << "Error " << "0x" << std::hex << code << ':' << description << "\n";
}

/**
 * Initializes the GLFW window
 * @return 0 if successful, 1 if not
 */
unsigned GLFWApplication::Init() {
    glfwSetErrorCallback(GLFWErrorCallback);
	if (!glfwInit()){
        std::cin.get();
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

    if (window == nullptr)
    {

        glfwTerminate();

        std::cin.get();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    std::cout << "\tVendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "\tRenderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "\tOpenGL version: " << glGetString(GL_VERSION) << "\n";

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    return 0;
}
