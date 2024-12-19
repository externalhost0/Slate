//
// Created by Hayden Rivas on 11/25/24.
//

#ifndef SLATEEDITOR_SHADERLIBRARY_H
#define SLATEEDITOR_SHADERLIBRARY_H
#include "Ref.h"
#include "IManager.h"
#include "Shader.h"

namespace Slate {
    class ShaderLibrary {
    public:

        void Add(const std::string& name, const Ref<Shader>& shader);
        bool Exists(const std::string& name) const;
        Ref<Shader> Load(const std::string& name, const std::string& vertSrc, const std::string& fragSrc);
        Ref<Shader> Get(const std::string& name);

        void Setup();
    private:
//        void Initialize() override {};
//        void Shutdown() override {};
        friend class Application;
    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };

}
#endif //SLATEEDITOR_SHADERLIBRARY_H
