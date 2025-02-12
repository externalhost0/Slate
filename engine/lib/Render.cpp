//
// Created by Hayden Rivas on 10/30/24.
//
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

#include "Slate/Debug.h"
#include "Slate/Render.h"

namespace Slate {
    // these are all wrappers that are seperated in case of other api abstraction
    // basic clear function
    void RenderManager::Clear(uint32_t mask) { GL_CALL(glClear(mask)); }
    // without specifying color, uses last color used
    void RenderManager::ClearColor() const { GL_CALL(glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a)); }
    void RenderManager::ClearStencil(int val) { GL_CALL(glClearStencil(val)); }

}