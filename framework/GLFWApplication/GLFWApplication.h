#ifndef GLFWAPPLICATION_H
#define GLFWAPPLICATION_H

#include <string>
#include <GLFW/glfw3.h>


class GLFWApplication
{
protected:
    int width;
    int height;
	GLFWwindow* window;
    std::string name;

public:
	GLFWApplication(){ };
	GLFWApplication(const std::string& name, const std::string& version);
	~GLFWApplication();

	// Initialization 
	virtual unsigned Init(); // Virtual function with default behavior
	// Run function
	virtual unsigned Run() const = 0; // Pure virtual function, it must be redefined

};

#endif