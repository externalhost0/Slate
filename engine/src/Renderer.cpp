//
// Created by Hayden Rivas on 10/30/24.
//
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <Slate/Renderer.h>


namespace Slate {
    // these are all wrappers that are seperated in case of other api abstraction
    // basic clear function
    void Renderer::Clear(uint32_t mask) { glClear(mask); }
    // without specifying color, uses last color used
    void Renderer::ClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }
    void Renderer::ClearStencil(int val) { glClearStencil(val); }

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
        // enables alpha testing / blend
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    void Renderer::Shutdown() {

    }

}