//
// Created by Hayden Rivas on 10/30/24.
//
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <Slate/Renderer.h>


namespace Slate {

    void AddVertexArray() {

    }

    // the good generateBuffer function
    void Renderer::BindBuffer(const float *vertices, size_t vertexCount, const unsigned int *indices, size_t indiceCount) {
        // buffer for verts
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // ensure that they are in bytes by multiplying by size of type
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexCount * sizeof(float)), vertices,GL_STATIC_DRAW);

        // buffer for elements
        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indiceCount * sizeof(unsigned int)), indices,GL_STATIC_DRAW);

        // layout 0: position, controls how vertices are read
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
        glEnableVertexAttribArray(0);
    }
    // buffer for quad as of now
    void Renderer::BindBufferWithTexture(const float *vertices, size_t vertexCount, const unsigned int *indices, size_t indiceCount) {
        // buffer for verts
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // ensure that they are in bytes by multiplying by size of type
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexCount * sizeof(vertices[0])), vertices,GL_STATIC_DRAW);

        // buffer for elements
        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indiceCount * sizeof(indices[0])), indices,GL_STATIC_DRAW);

        // layout 0: position, controls how vertices are read
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
        glEnableVertexAttribArray(0);

        // layout 1: texture coordinates, controls how textures are placed and sampled
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // these are all wrappers that are seperated in case of other api abstraction
    // basic clear function
    void Renderer::Clear(uint32_t mask) { glClear(mask); }
    // without specifying color, uses last color used
    void Renderer::ClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }
    void Renderer::ClearStencil(int val) { glClearStencil(val); }

    void Renderer::GenerateVertexArray(unsigned int& object) {
        glGenVertexArrays(1, &object);
    }
    void Renderer::BindVertexArray(unsigned int& object) {
        glBindVertexArray(object);
    }
    void Renderer::UnbindVertexArray() {
        glBindVertexArray(0);
    }

    void Renderer::Init() {
        // ONLY FLAGS FOR OPENGL
        // enable depth testing
        glEnable(GL_DEPTH_TEST);
        // enable stencil testing (for outlines)
        glEnable(GL_STENCIL_TEST);
        // set culling so only front faces show
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        // enable srgb for nicer colors and their transitions
        glEnable(GL_SRGB);
        // enables alpha testing / blend
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    void Renderer::Shutdown() {

    }

    void Renderer::SetClearColor(const glm::vec4 &color) { m_ClearColor = color; }


}