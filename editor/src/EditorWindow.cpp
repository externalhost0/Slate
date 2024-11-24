//
// Created by Hayden Rivas on 10/30/24.
//

#include <cstdio>
#include <GL/glew.h>

#include "EditorWindow.h"
#include "Slate/Input.h"
#include "../../engine/src/errors.h"

namespace Slate {
    bool EditorWindow::CreateWindow() {
        glfwInit();

        // some hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        // m_EditorWindow mode setup
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);

        int width, height;
        width = mode->width;
        height = mode->height;
        if (m_VideoMode == BORDERLESS_FULLSCREEN) {
            monitor = nullptr;
        } else if (m_VideoMode == WINDOWED) {
            monitor = nullptr;
            width = m_WindowWidth;
            height = m_WindowHeight;
        }

        m_Window = glfwCreateWindow(width, height, m_WindowTitle.c_str(), monitor, nullptr);
        glfwMakeContextCurrent(m_Window);
        Input::SetWindow(*m_Window);

        if (glewInit() != GLEW_OK) {
            printf("GLEW FAILED TO BE INITIALIZED");
            return EXIT_FAILURE;
        }

        // if everything went the way it should, then give the class all the pulled values
        m_RefreshRate = mode->refreshRate;
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        glfwWindowHint(GLFW_RESIZABLE, m_IsResizeable);
        glfwSwapInterval(m_VSync); // vsync
        return EXIT_SUCCESS;
    }

    void EditorWindow::SetVideoMode(VIDEO_MODE mode) {
        if (mode == BORDERLESS_FULLSCREEN) {
            m_WindowMonitor = nullptr;
        }
        if (mode == WINDOWED) {
            m_WindowMonitor = nullptr;
            m_WindowWidth = 1200;
            m_WindowHeight = 700;
        }
        int xpos, ypos;
        glfwGetWindowPos(m_Window, &xpos, &ypos);
        glfwSetWindowMonitor(m_Window, m_WindowMonitor, xpos, ypos, m_WindowWidth, m_WindowHeight, m_RefreshRate);
    }

    GLFWwindow* EditorWindow::GetWindow() {
        EXPECT(m_Window != nullptr, "Settings Window Doesn't Exist!");
        return m_Window;
    }
    void EditorWindow::SetVsync(int refresh) {
        m_VSync = refresh;
        glfwSwapInterval(refresh); // vsync
    }
}