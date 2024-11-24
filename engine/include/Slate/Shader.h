//
// Created by Hayden Rivas on 11/22/24.
//

#ifndef SLATEEDITOR_SHADER_H
#define SLATEEDITOR_SHADER_H

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <unordered_map>

namespace Slate {
    class Shader {
    public:
        Shader() = default;
        ~Shader() = default;
        Shader(const std::string& vertexfile, const std::string& fragfile);

        // exactly what it sounds like
        void UseProgram() const;
        unsigned int GetProgramIDCopy() const { return m_ProgramID; }
        // uniform setter methods
        void setBool(const char* name, bool value) const;
        void setInt(const char* name, int value) const;
        void setFloat(const char* name, float value) const;
        void setVec2(const char* name, const glm::vec2 *value) const;
        void setVec3(const char* name, const glm::vec3 *value) const;
        void setVec4(const char* name, const glm::vec4 *value) const;
        void setVec2(const char* name, float x, float y) const;
        void setVec3(const char* name, float x, float y, float z) const;
        void setVec4(const char* name, float x, float y, float z, float w) const;
        void setMat2(const char* name, const glm::mat2 &mat) const;
        void setMat3(const char* name, const glm::mat3 &mat) const;
        void setMat4(const char* name, const glm::mat4 &mat) const;

        // uniform data getter methods
        bool getBool(const char* name) const;
        int getInt(const char* name) const;
        float getFloat(const char* name) const;
        glm::vec2 getVe2(const char* name) const;
        glm::vec3 getVec3(const char* name) const;
        glm::vec4 getVec4(const char* name) const;

        std::string m_VertexFile;
        std::string m_FragmentFile;
    private:
        int GetUniformLocation(const char* name) const;
        mutable std::unordered_map<const char*, int> m_UniformLocationCache;
        unsigned int m_ProgramID{}; // acts like an id for shaders? should i use an independent integar variable for that?
        friend class Mesh; // they are tied very close to each other
    };
}

#endif //SLATEEDITOR_SHADER_H
