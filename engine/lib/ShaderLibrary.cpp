//
// Created by Hayden Rivas on 11/25/24.
//

#include "Slate/Expect.h"
#include "Slate/Files.h"
#include "Slate/ShaderLibrary.h"

namespace Slate {
    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertSrc, const std::string& fragSrc) {
        auto shader = CreateRef<Shader>(name, ToDirectory(vertSrc), ToDirectory(fragSrc));
        Add(name, shader);
        return shader;
    }

    // not used until shader system accepts singles files that contain both vert and frag
//    void ShaderLibrary::Add(const Ref<Shader> &shader) {
//        auto& name = shader->GetName();
//        Add(name, shader);
//    }

    void ShaderLibrary::Add(const std::string &name, const Ref<Shader> &shader) {
        EXPECT(!Exists(name), "Shader already exists!");
        m_Shaders[name] = shader;
    }
    Ref<Shader> ShaderLibrary::Get(const std::string &name) {
        EXPECT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const {
        return m_Shaders.find(name) != m_Shaders.end();
    }

    void ShaderLibrary::Setup() {
        Load("null", "assets/shaders/vertex/static_PNT.vert", "assets/shaders/fragment/null.frag");
        Load("solid_color", "assets/shaders/vertex/static_PNT.vert", "assets/shaders/fragment/entity_solidcolor.frag");
        Load("normals_only", "assets/shaders/vertex/static_PNT.vert", "assets/shaders/fragment/entity_normalsonly.frag");

    }

}