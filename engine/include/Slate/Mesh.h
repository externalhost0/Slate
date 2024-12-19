//
// Created by Hayden Rivas on 11/21/24.
//
#ifndef SLATEEDITOR_MESH_H
#define SLATEEDITOR_MESH_H

#include <glad/glad.h>
#include "BufferObject.h"

namespace Slate {
    class Mesh {
    public:
        Mesh(const Vertices& vertices, const LayoutBuffer& layout);
        Mesh(const Vertices& vertices, const LayoutBuffer& layout, GLint mode)
        : Mesh(vertices, layout) { this->mode = mode; }
        Mesh(const Vertices& vertices, const Elements& element, const LayoutBuffer& layout);
    public:
        void DrawMesh() const;
        void Bind() const;
        void Unbind() const;
    private:
        void AddVertexAttribArray(const BufferElement& element);
    private:
        GLint mode{GL_TRIANGLES};
        unsigned int VAO{}, VBO{}, EBO{};
        unsigned int m_AttribIndex = 0; // all indexs always start at 0
        Vertices m_VertexData{};
        Elements m_ElementData{};
        LayoutBuffer m_Layout{};
    };
}


#endif //SLATEEDITOR_MESH_H
