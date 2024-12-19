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

#include "Ref.h"

namespace Slate {
    class Shader {
    public:
        virtual ~Shader() = default;
        // implement create api when vulkan is added
        Shader(std::string name, const std::string& vertexfile, const std::string& fragfile);

        // exactly what it sounds like
        void UseProgram() const;
        unsigned int GetProgramIDCopy() const { return m_ProgramID; };

        const std::string& GetName() const { return m_Name; };

        // uniform setter methods
        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetVec2(const std::string& name, const glm::vec2 *value) const;
        void SetVec3(const std::string& name, const glm::vec3 *value) const;
        void SetVec4(const std::string& name, const glm::vec4 *value) const;
        void SetVec2(const std::string& name, float x, float y) const;
        void SetVec3(const std::string& name, float x, float y, float z) const;
        void SetVec4(const std::string& name, float x, float y, float z, float w) const;
        void SetMat2(const std::string& name, const glm::mat2 &mat) const;
        void SetMat3(const std::string& name, const glm::mat3 &mat) const;
        void SetMat4(const std::string& name, const glm::mat4 &mat) const;

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
        std::string m_Name;
        mutable std::unordered_map<std::string, int> m_UniformLocationCache;
        unsigned int m_ProgramID{}; // acts like an id for shaders? should i use an independent integar variable for that?
        friend class Mesh; // they are tied very close to each other
    private:
        unsigned int CompileProgram(const std::string& vfp, const std::string& ffp);
        int GetUniformLocation(const std::string& name) const;
    };
}

#endif //SLATEEDITOR_SHADER_H
