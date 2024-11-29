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
        Mesh(const Vertices& vertice, const Elements& element, const LayoutBuffer& layout) {
            m_VertexData = vertice;
            m_ElementData = element;
            m_Layout = layout;

            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(m_VertexData.m_Size), m_VertexData.m_Data.data(), GL_STATIC_DRAW);

            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(m_ElementData.m_Size), m_ElementData.m_Data.data(), GL_STATIC_DRAW);

            for (const auto& el : layout)
                AddVertexAttribArray(el);
            glBindVertexArray(0);

        }
        void DrawMesh() const;
    private:
        void AddVertexAttribArray(const BufferElement& element);
    private:
        unsigned int VAO{}, VBO{}, EBO{};
        unsigned int m_AttribIndex = 0; // all indexs always start at 0
        Vertices m_VertexData{};
        Elements m_ElementData{};
        LayoutBuffer m_Layout{};
    };
}


#endif //SLATEEDITOR_MESH_H
