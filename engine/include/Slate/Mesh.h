//
// Created by Hayden Rivas on 11/21/24.
//

#ifndef SLATEEDITOR_MESH_H
#define SLATEEDITOR_MESH_H

#include "Shader.h"

#include <vector>
namespace Slate {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
    };
    class Mesh {
    public:
        Mesh(const float* vertices, size_t verticeCount, const unsigned int* indices, size_t indiceCount);
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        std::vector<Vertex> m_Vertices;
        std::vector<unsigned int> m_Indices;

        void DrawMesh() const;
    private:
        // vertex array, vertex buffer, element buffer
        unsigned int VAO{}, VBO{}, EBO{};
        void BuildMesh();
    };
}


#endif //SLATEEDITOR_MESH_H
