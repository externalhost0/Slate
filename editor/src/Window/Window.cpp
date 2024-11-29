//
// Created by Hayden Rivas on 11/26/24.
//
#include <glad/glad.h>
#include <iostream>
#include <Slate/Input.h>
#include <SLate/Expect.h>

#include "Window.h"

namespace Slate {
    void Window::BuildWindow() {
        if (!glfwGetCurrentContext())
            glfwInit();

        // some global hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        // some more hints
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        m_WindowSpecification.m_RefreshRate = mode->refreshRate;

        // some small testing to set fullscreen/borderless/windowed
        // we take everything as is recieved from mode if Fullscreen is asked
        int width = mode->width;
        int height = mode->height;
        if (m_WindowSpecification.m_VideoMode == BORDERLESS_FULLSCREEN) {
            monitor = nullptr;
        } else if (m_WindowSpecification.m_VideoMode == WINDOWED) {
            monitor = nullptr;
            width = m_WindowSpecification.m_WindowWidth;
            height = m_WindowSpecification.m_WindowHeight;
        }

        // actual making and setting of window
        m_NativeWindow = glfwCreateWindow(width, height, m_WindowSpecification.m_WindowTitle.c_str(), monitor, nullptr);
        glfwMakeContextCurrent(m_NativeWindow);

        Input::SetWindow(m_NativeWindow); // might change this little line later!!

        // init glad!
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            EXPECT(false, "Failed to initialize GLAD\nThis is likely and issue with your OpenGL version.\n")


        // if everything went the way it should, then give the class all the pulled values
        glfwWindowHint(GLFW_RESIZABLE, m_WindowSpecification.m_IsResizeable);
        glfwSwapInterval(m_WindowSpecification.m_VSync);
    }
}