//
// Created by Hayden Rivas on 11/25/24.
//

#include "Slate/Expect.h"
#include "Slate/ShaderManager.h"

namespace Slate {
    Ref<Shader> ShaderManager::Load(const std::string& name, const std::string& vertSrc, const std::string& fragSrc) {
        auto shader = CreateRef<Shader>(name, vertSrc, fragSrc);
        Add(name, shader);
        return shader;
    }

    // not used until shader system accepts singles files that contain both vert and frag
//    void ShaderManager::Add(const Ref<Shader> &shader) {
//        auto& name = shader->GetName();
//        Add(name, shader);
//    }

    void ShaderManager::Add(const std::string &name, const Ref<Shader> &shader) {
        EXPECT(!Exists(name), "Shader already exists!");
        m_Shaders[name] = shader;
    }
    Ref<Shader> ShaderManager::Get(const std::string &name) {
        EXPECT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }

    bool ShaderManager::Exists(const std::string& name) const {
        return m_Shaders.find(name) != m_Shaders.end();
    }

    void ShaderManager::Setup() {
        Load("null", "../editor/assets/shaders/static.vert", "../editor/assets/shaders/null.frag");
        Load("solid_color", "../editor/assets/shaders/static.vert", "../editor/assets/shaders/solid_color.frag");
        Load("normals_only", "../editor/assets/shaders/static.vert", "../editor/assets/shaders/normalsonly.frag");
        Load("overdraw", "../editor/assets/shaders/static.vert", "../editor/assets/shaders/overdraw.frag");

    }

}