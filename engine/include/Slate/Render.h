//
// Created by Hayden Rivas on 10/30/24.
//
#pragma once

#include <glad/glad.h>
#include "Scene.h"
#include "Ref.h"
#include "ShaderLibrary.h"
#include "IManager.h"

namespace Slate {
    class RenderManager : public IManager {
    public:

        void Clear(uint32_t mask);
        void ClearColor() const; // uses clearcolor member instead of passing color manually
        void ClearStencil(int val = 0x00);

        ShaderLibrary& GetShaderManager() { return *m_ShaderLib; };
    private:
        void Initialize() override {
            m_ShaderLib = new ShaderLibrary;
        };
        void Shutdown() override {};
        friend class Application;
    public:
        glm::vec4 m_ClearColor = {0.1f, 0.1f, 0.1f, 1.0f};
    private:
        ShaderLibrary* m_ShaderLib;
    };


}
