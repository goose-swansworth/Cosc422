

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader() {
        ID = glCreateProgram();
    }

    void attachShader(GLenum shaderType, string filename) {
        ifstream shaderFile(filename.c_str());
        if(!shaderFile.good()) cout << "Error opening shader file." << endl;
        stringstream shaderData;
        shaderData << shaderFile.rdbuf();
        shaderFile.close();
        string shaderStr = shaderData.str();
        const char* shaderTxt = shaderStr.c_str();

        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderTxt, NULL);
        glCompileShader(shader);
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
            const char *strShaderType = NULL;
            cerr <<  "Compile failure in shader: " << strInfoLog << endl;
            delete[] strInfoLog;
        }
        glAttachShader(ID, shader);
    }

    void link() {
        glLinkProgram(ID);
        GLint status;
        glGetProgramiv(ID, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            GLint infoLogLength;
            glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetProgramInfoLog(ID, infoLogLength, NULL, strInfoLog);
            fprintf(stderr, "Linker failure: %s\n", strInfoLog);
            delete[] strInfoLog;
        }
    }

    // activate the shader
    // ------------------------------------------------------------------------
    void use() 
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void setMat4(const std::string &name, glm::mat4 mat) {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setVec3(const std::string &name, glm::vec3 vec) {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }

    void setVec4(const std::string &name, glm::vec4 vec) {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

};
#endif

