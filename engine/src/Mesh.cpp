//
// Created by Hayden Rivas on 11/21/24.
//
#include <GL/glew.h>
#include "Slate/Mesh.h"
#include <iostream>

namespace Slate {
    // for pre defined meshes
    Mesh::Mesh(const float *vertices, size_t verticeCount, const unsigned int *indices, size_t indiceCount) {
        size_t stride = 8; // 3 (Position) + 3 (Normal) + 2 (TexCoord)
        m_Vertices.reserve(verticeCount / stride);

        for (size_t i = 0; i < verticeCount; i += stride) {
            Vertex vertex{};
            vertex.Position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
            vertex.Normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
            vertex.TexCoord = glm::vec2(vertices[i + 6], vertices[i + 7]);
            m_Vertices.push_back(vertex);
        }

        m_Indices.assign(indices, indices + indiceCount);

        BuildMesh();
    }

    // for imported meshes
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
        this->m_Vertices = vertices;
        this->m_Indices = indices;

        BuildMesh();
    }

    void Mesh::BuildMesh() {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Vertices.size() * sizeof(Vertex)), &m_Vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Indices.size() * sizeof(unsigned int)), &m_Indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

        glBindVertexArray(0);
    }

    void Mesh::DrawMesh() const {
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

}