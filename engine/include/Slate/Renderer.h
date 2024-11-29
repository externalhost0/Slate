//
// Created by Hayden Rivas on 10/30/24.
//
#ifndef SLATE_GRAPHICS_H
#define SLATE_GRAPHICS_H

#include "Scene.h"
#include "Ref.h"
#include "ShaderManager.h"

namespace Slate {
    class Renderer {
    public:
        static void Init();
        static void Shutdown();

        // not sure how transparency works in this
        static inline glm::vec4 m_ClearColor = {0.1f, 0.1f, 0.1f, 1.0f};


        static void Clear(uint32_t mask);
        static void ClearColor(float r = m_ClearColor.r, float g = m_ClearColor.g, float b = m_ClearColor.b, float a = m_ClearColor.a);

        static void ClearStencil(int val = 0x00);

        static ShaderManager& GetShaderManager() { return m_ShaderManager; };
    private:
        static inline ShaderManager m_ShaderManager;
    };
}
#endif //SLATE_GRAPHICS_H
