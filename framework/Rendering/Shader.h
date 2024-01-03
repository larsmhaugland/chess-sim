#ifndef SHADER_H
#define SHADER_H


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>


class Shader
{
private: 
    
    unsigned int ID;

public:
    // constructor reads and builds the shader
    Shader() {};
    Shader(const std::string vertexShader, const std::string fragmentShader);
    ~Shader();

    // use/activate the shader
        void use();
    // utility uniform functions
        int retID(){return ID;}
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setMat4(const std::string& name, const glm::mat4& mat)const;
        void setUniform4f(const std::string& name, glm::vec4 v);
        void setUniform3f(const std::string& name, glm::vec3 v);
        void setUniform2f(const std::string& name, glm::vec2 v);

    void setDouble(const char *string, double d);

private:
    int GetUniformLocation(const std::string& name);
    GLuint compileShader(const std::string vertexShader, const std::string& fragmentShader);
};

#endif