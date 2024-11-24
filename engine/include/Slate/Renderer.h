//
// Created by Hayden Rivas on 10/30/24.
//
#ifndef SLATE_GRAPHICS_H
#define SLATE_GRAPHICS_H

#include "Slate/Scene.h"
#include "Slate/Ref.h"

namespace Slate {
    class Renderer {
    public:
        Renderer() = default;
        ~Renderer() = default;

        static void Init();
        static void Shutdown();

        // not sure how transparency works in this
        static inline glm::vec4 m_ClearColor = {0.20f, 0.19f, 0.21f, 1.0f};


        static void Clear(uint32_t mask);
        static void ClearColor(float r = m_ClearColor.r, float g = m_ClearColor.g, float b = m_ClearColor.b, float a = m_ClearColor.a);
        static void SetClearColor(const glm::vec4& color);

        static void BindBufferWithTexture(const float *vertices, size_t vertexCount, const unsigned int *indices, size_t indiceCount);
        static void BindBuffer(const float *vertices, size_t vertexCount, const unsigned int *indices, size_t indiceCount);

        static void BindVertexArray(unsigned int& object);
        static void GenerateVertexArray(unsigned int& object);
        static void UnbindVertexArray();


    private:
        friend class EditorLayer;
        static void ClearStencil(int val = 0x00);
    };
}
#endif //SLATE_GRAPHICS_H
