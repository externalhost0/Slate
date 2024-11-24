//
// Created by Hayden Rivas on 10/29/24.
//

#include <GL/glew.h>
#include <fstream>
#include <sstream>

#include "Slate/Files.h"

namespace Slate {
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
        char infoLog[512];
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
    unsigned int Files::generateProgram(const std::string& vfp, const std::string& ffp) {
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
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
        if (!result) {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
            printf("(This is likely an error with how the shader itself is written.)\n");
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteProgram(shaderProgram);
            return 1;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // test this out, remove later if necessary
//        glUseProgram(shaderProgram);

        return shaderProgram;
    }

}