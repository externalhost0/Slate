//
// Created by Hayden Rivas on 11/1/24.
//
#include "Slate/Shader.h"
#include "Slate/Files.h"
#include "Slate/Expect.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <utility>
#include <iostream>

namespace Slate {
    // basic uniforms
    void Shader::setBool(const char *name, bool value) const {
        // does the same thing as an int set cause there is no bool setter, i just want it to be pretty :)
        this->setInt(name, (int) value);
    }
    void Shader::setInt(const char *name, int value) const {
        // ALWAYS CHECK FOR ERRORS RETARD
        glProgramUniform1i(this->m_ProgramID,GetUniformLocation(name), value);
    }
    void Shader::setFloat(const char *name, float value) const {
        glProgramUniform1f(this->m_ProgramID, GetUniformLocation(name), value);
    }
    // vector uniforms
    void Shader::setVec2(const char *name, const glm::vec2 *value) const {
        glProgramUniform2fv(this->m_ProgramID, GetUniformLocation(name), 1, &(value->x));
    }
    void Shader::setVec3(const char *name, const glm::vec3 *value) const {
        glProgramUniform3fv(this->m_ProgramID, GetUniformLocation(name), 1, &(value->x));
    }
    void Shader::setVec4(const char *name, const glm::vec4 *value) const {
        glProgramUniform4fv(this->m_ProgramID, GetUniformLocation(name), 1, &(value->x));
    }
    void Shader::setVec2(const char *name, float x, float y) const {
        glProgramUniform2f(this->m_ProgramID, GetUniformLocation(name), x, y);
    }
    void Shader::setVec3(const char *name, float x, float y, float z) const {
        glProgramUniform3f(this->m_ProgramID, GetUniformLocation(name), x, y, z);
    }
    void Shader::setVec4(const char *name, float x, float y, float z, float w) const {
        glProgramUniform4f(this->m_ProgramID, GetUniformLocation(name), x, y, z, w);
    }
    // matrix uniforms
    void Shader::setMat2(const char *name, const glm::mat2 &mat) const {
        glProgramUniformMatrix2fv(this->m_ProgramID, GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    void Shader::setMat3(const char *name, const glm::mat3 &mat) const {
        glProgramUniformMatrix3fv(this->m_ProgramID, GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    void Shader::setMat4(const char *name, const glm::mat4 &mat) const {
        glProgramUniformMatrix4fv(this->m_ProgramID, GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }

    Shader::Shader(std::string name, const std::string& vertexfile, const std::string& fragfile)
    : m_Name(std::move(name)), m_VertexFile(vertexfile), m_FragmentFile(fragfile)
    {
        m_ProgramID = CompileProgram(vertexfile, fragfile);
    }

    void Shader::UseProgram() const {
        glUseProgram(m_ProgramID);
    }

    // getters
    bool Shader::getBool(const char *name) const {
        return getInt(name);
    }
    int Shader::getInt(const char* name) const {
        GLint value;
        glGetUniformiv(m_ProgramID, GetUniformLocation(name), &value);
        return value;
    }
    float Shader::getFloat(const char *name) const {
        GLfloat value;
        glGetUniformfv(m_ProgramID, GetUniformLocation(name), &value);
        return value;
    }
    glm::vec2 Shader::getVe2(const char* name) const {
        GLfloat values[2];
        glGetUniformfv(m_ProgramID, GetUniformLocation(name), values);
        return {values[0], values[1]};
    }
    glm::vec3 Shader::getVec3(const char* name) const {
        GLfloat values[3];
        glGetUniformfv(m_ProgramID, GetUniformLocation(name), values);
        return {values[0], values[1], values[2]};
    }
    glm::vec4 Shader::getVec4(const char* name) const {
        GLfloat values[4];
        glGetUniformfv(m_ProgramID, GetUniformLocation(name), values);
        return {values[0], values[1], values[2], values[3]};
    }
    // cache locations
    int Shader::GetUniformLocation(const char *name) const {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];
        int location = glGetUniformLocation(m_ProgramID, name);
        m_UniformLocationCache[name] = location;
        return location;
    }

    // helper functions for compilation
    std::string readShaderFile(const std::string& fileName) {
        const std::string& filePath = fileName;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            printf("ERROR::SHADER::FILE_NOT_FOUND\n");
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        if (buffer.str().empty()) {
            printf("ERROR::SHADER::FILE_IS_EMPTY\n");
            return "";
        }
        return buffer.str();
    }
    unsigned int compileShader(unsigned int shadertype, const char *shadersource) {
        unsigned int shader = glCreateShader(shadertype);
        glShaderSource(shader, 1, &shadersource, nullptr);
        glCompileShader(shader);

        int result;
        char infoLog[256];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (!result) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            printf("ERROR::SHADER::%s::COMPILATION_FAILED\n%s\n",
                   shadertype == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT", infoLog);
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }
    unsigned int Shader::CompileProgram(const std::string& vfp, const std::string& ffp) {
        // shader compilation
        std::string vertexShaderSource = readShaderFile(vfp);
        unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
        std::string fragmentShaderSource = readShaderFile(ffp);
        unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

        // link shaders
        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);

        glLinkProgram(shaderProgram);
        glValidateProgram(shaderProgram);

        // check for linking errors
        int result;
        char infoLog[256];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
        // if compile status was not successful
        if (!result) {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
            printf("(This is likely an error with how the shader itself is written.)\n");
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteProgram(shaderProgram);
            return 1;
        }
        // detach, not delete, in case we want to reuse shader ids for other programs
        glDetachShader(shaderProgram, vertexShader);
        glDetachShader(shaderProgram, fragmentShader);


        return shaderProgram;
    }
}