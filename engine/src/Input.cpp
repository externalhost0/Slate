//
// Created by Hayden Rivas on 10/30/24.
//

#include "Slate/Input.h"

namespace Slate {

    bool Input::IsKeyPressed(const int key) {
        auto state = glfwGetKey(m_Window, static_cast<int32_t>(key));
        return state == GLFW_PRESS;
    }
    bool Input::IsMouseButtonPressed(const int button) {
        auto state = glfwGetMouseButton(m_Window, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
    }
    glm::vec2 Input::GetMousePosition() {
        double x_pos, y_pos;
        glfwGetCursorPos(m_Window, &x_pos, &y_pos);
        return {(float) x_pos, (float) y_pos };
    }

    void Input::SetWindow(GLFWwindow* window) {
        m_Window = window;
    }

    int Input::GetInputMode() {
        return glfwGetInputMode(m_Window, GLFW_CURSOR);
    }
    void Input::SetInputMode(int mode) {
        glfwSetInputMode(m_Window, GLFW_CURSOR, mode);
    }

}