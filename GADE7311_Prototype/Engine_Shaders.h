#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>


class EngineShader
{
public:
    unsigned int ShaderID;

    EngineShader(const char* vertexFile, const char* fragmentsFile)
    {
        
        std::string vertices;
        std::string fragments;
        std::ifstream vertexShaderReader;
        std::ifstream fragmentShaderReader;
        
        vertexShaderReader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderReader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vertexShaderReader.open(vertexFile);
            fragmentShaderReader.open(fragmentsFile);
            std::stringstream vertexStreaming, fragmentStreaming;
            
            vertexStreaming << vertexShaderReader.rdbuf();
            fragmentStreaming << fragmentShaderReader.rdbuf();
            
            vertices = vertexStreaming.str();
            fragments = fragmentStreaming.str();
            
            vertexShaderReader.close();
            fragmentShaderReader.close();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "Problem reading from file" << std::endl;
        }
        const char* vShaderCode = vertices.c_str();
        const char* fShaderCode = fragments.c_str();

        unsigned int vertVal, fragVal;
        int complete;
        char infoLog[512];

        vertVal = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertVal, 1, &vShaderCode, NULL);
        glCompileShader(vertVal);

        fragVal = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragVal, 1, &fShaderCode, NULL);
        glCompileShader(fragVal);

        ShaderID = glCreateProgram();
        glAttachShader(ShaderID, vertVal);
        glAttachShader(ShaderID, fragVal);
        glLinkProgram(ShaderID);

        glGetProgramiv(ShaderID, GL_LINK_STATUS, &complete);
        if (!complete)
        {
            glGetProgramInfoLog(ShaderID, 512, NULL, infoLog);
            std::cout << "Failed Shader Link\n" << infoLog << std::endl;
        }
        glDeleteShader(vertVal);
        glDeleteShader(fragVal);
    }
    
    void use()
    {
        glUseProgram(ShaderID);
    }
    
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ShaderID, name.c_str()), (int)value);
    }
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ShaderID, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ShaderID, name.c_str()), value);
    }

    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(ShaderID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ShaderID, name.c_str()), x, y);
    }
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ShaderID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ShaderID, name.c_str()), x, y, z);
    }
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(ShaderID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ShaderID, name.c_str()), x, y, z, w);
    }
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};

#endif