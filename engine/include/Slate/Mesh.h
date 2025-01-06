//
// Created by Hayden Rivas on 11/21/24.
//
#pragma once

#include <glad/glad.h>
#include "BufferObject.h"

namespace Slate {
    class Mesh {
    public:
        Mesh() = default;

        Mesh(const Vertices& vertices, const LayoutBuffer& layout, GLint mode = GL_TRIANGLES);
        Mesh(const Vertices& vertices, const Elements& element, const LayoutBuffer& layout, GLint mode = GL_TRIANGLES);
    public:
        void BuildMesh() const;
    public:
        void DrawMesh() const;
        void Bind() const;
        void Unbind() const;
        Vertices m_VertexData{};
        Elements m_ElementData{};
        LayoutBuffer m_Layout{};
    private:
        void AddVertexAttribArray(const BufferElement& element);
    private:
        GLint mode{GL_TRIANGLES};
        unsigned int VAO{}, VBO{}, EBO{};
        unsigned int m_AttribIndex = 0; // all indexs always start at 0
    };
}