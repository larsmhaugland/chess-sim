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
        int retID() const {return ID;}
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setUInt(const std::string& name, unsigned int value) const;
        void setFloat(const std::string& name, float value) const;
        void setMat4(const std::string& name, const glm::mat4& mat)const;
        void setUniform4f(const std::string& name, glm::vec4 v) const;
        void setUniform3f(const std::string& name, glm::vec3 v) const;
        void setUniform2f(const std::string& name, glm::vec2 v) const;
        void setDouble(const char *string, double d) const;

private:
    int GetUniformLocation(const std::string& name) const;
    static GLuint compileShader(const std::string& vertexShader, const std::string& fragmentShader);
};

#endif