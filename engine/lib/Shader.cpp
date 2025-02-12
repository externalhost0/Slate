//
// Created by Hayden Rivas on 11/1/24.
//
#include <glad/glad.h>

#include "Slate/Shader.h"
#include "Slate/Files.h"
#include "Slate/Debug.h"

#include <fstream>
#include <sstream>
#include <utility>

namespace Slate {
    // basic uniforms
    void Shader::SetBool(const std::string& name, bool value) const {
        // does the same thing as an int set cause there is no bool setter, i just want it to be pretty :)
        this->SetInt(name, static_cast<int>(value));
    }
    void Shader::SetInt(const std::string& name, int value) const {
        // ALWAYS CHECK FOR ERRORS RETARD
        GL_CALL(glProgramUniform1i(this->m_ProgramID, GetUniformLocation(name), value));
    }
    void Shader::SetFloat(const std::string& name, float value) const {
        GL_CALL(glProgramUniform1f(this->m_ProgramID, GetUniformLocation(name), value));
    }
    // vector uniforms
    void Shader::SetVec2(const std::string& name, const glm::vec2 *value) const {
        GL_CALL(glProgramUniform2fv(this->m_ProgramID, GetUniformLocation(name), 1, &(value->x)));
    }
    void Shader::SetVec3(const std::string& name, const glm::vec3 *value) const {
        GL_CALL(glProgramUniform3fv(this->m_ProgramID, GetUniformLocation(name), 1, &(value->x)));
    }
    void Shader::SetVec4(const std::string& name, const glm::vec4 *value) const {
        GL_CALL(glProgramUniform4fv(this->m_ProgramID, GetUniformLocation(name), 1, &(value->x)));
    }
    void Shader::SetVec2(const std::string& name, float x, float y) const {
        GL_CALL(glProgramUniform2f(this->m_ProgramID, GetUniformLocation(name), x, y));
    }
    void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
        GL_CALL(glProgramUniform3f(this->m_ProgramID, GetUniformLocation(name), x, y, z));
    }

    void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const {
        GL_CALL(glProgramUniform4f(this->m_ProgramID, GetUniformLocation(name), x, y, z, w));
    }
    // matrix uniforms
    void Shader::SetMat2(const std::string& name, const glm::mat2 &mat) const {
        GL_CALL(glProgramUniformMatrix2fv(this->m_ProgramID, GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
    }
    void Shader::SetMat3(const std::string& name, const glm::mat3 &mat) const {
        GL_CALL(glProgramUniformMatrix3fv(this->m_ProgramID, GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
    }
    void Shader::SetMat4(const std::string& name, const glm::mat4 &mat) const {
        GL_CALL(glProgramUniformMatrix4fv(this->m_ProgramID, GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
    }

    Shader::Shader(std::string name, const std::string& vertexfile, const std::string& fragfile)
    : m_Name(std::move(name)), m_VertexFile(vertexfile), m_FragmentFile(fragfile)
    {
        m_ProgramID = CompileProgram(vertexfile, fragfile);
    }

    void Shader::UseProgram() const {
        GL_CALL(glUseProgram(m_ProgramID));
    }

    // getters
    bool Shader::getBool(const char *name) const {
        return getInt(name);
    }
    int Shader::getInt(const char* name) const {
        GLint value;
        GL_CALL(glGetUniformiv(m_ProgramID, GetUniformLocation(name), &value));
        return value;
    }
    float Shader::getFloat(const char *name) const {
        GLfloat value;
        GL_CALL(glGetUniformfv(m_ProgramID, GetUniformLocation(name), &value));
        return value;
    }
    glm::vec2 Shader::getVe2(const char* name) const {
        GLfloat values[2];
        GL_CALL(glGetUniformfv(m_ProgramID, GetUniformLocation(name), values));
        return {values[0], values[1]};
    }
    glm::vec3 Shader::getVec3(const char* name) const {
        GLfloat values[3];
        GL_CALL(glGetUniformfv(m_ProgramID, GetUniformLocation(name), values));
        return {values[0], values[1], values[2]};
    }
    glm::vec4 Shader::getVec4(const char* name) const {
        GLfloat values[4];
        GL_CALL(glGetUniformfv(m_ProgramID, GetUniformLocation(name), values));
        return {values[0], values[1], values[2], values[3]};
    }

    // cache locations
    int Shader::GetUniformLocation(const std::string& name) const {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];
        int location = glGetUniformLocation(this->m_ProgramID, name.c_str());
        if (location == -1) {
            fprintf(stderr, "Warning: Uniform '%s' does not exist or is not active in the shader program: '%s'\n", name.c_str(), m_Name.c_str());
        }
        m_UniformLocationCache[name] = location;
        return location;
    }

    // helper functions for compilation
    std::string readShaderFile(const std::string& fileName) {
        const std::string& filePath = fileName;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            fprintf(stderr, "ERROR::SHADER::FILE_NOT_FOUND\n\t%s\n", fileName.c_str());
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        if (buffer.str().empty()) {
            fprintf(stderr, "ERROR::SHADER::FILE_IS_EMPTY\n\t%s\n", fileName.c_str());
            return "";
        }
        return buffer.str();
    }
    unsigned int compileShader(unsigned int shadertype, const char *shadersource) {
        unsigned int shader;
        GL_CALL(shader = glCreateShader(shadertype));
        GL_CALL(glShaderSource(shader, 1, &shadersource, nullptr));
        GL_CALL(glCompileShader(shader));

        int result;
        char infoLog[256];
        GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
        if (!result) {
            GL_CALL(glGetShaderInfoLog(shader, 512, nullptr, infoLog));
            fprintf(stderr, "ERROR::SHADER::%s::COMPILATION_FAILED\n\t%s\n", (shadertype==GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"), infoLog);
            GL_CALL(glDeleteShader(shader));
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
        unsigned int shaderProgram;
        GL_CALL(shaderProgram = glCreateProgram());
        GL_CALL(glAttachShader(shaderProgram, vertexShader));
        GL_CALL(glAttachShader(shaderProgram, fragmentShader));

        GL_CALL(glLinkProgram(shaderProgram));
        GL_CALL(glValidateProgram(shaderProgram));

        // check for linking errors
        int result;
        char infoLog[256];
        GL_CALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result));
        // if compile status was not successful
        if (!result) {
            GL_CALL(glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog));
            fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
            GL_CALL(glDeleteShader(vertexShader));
            GL_CALL(glDeleteShader(fragmentShader));
            GL_CALL(glDeleteProgram(shaderProgram));
            return 1; // EXIT_ERROR
        }
        // detach, not delete, in case we want to reuse shader ids for other programs
        GL_CALL(glDetachShader(shaderProgram, vertexShader));
        GL_CALL(glDetachShader(shaderProgram, fragmentShader));

        return shaderProgram;
    }

}