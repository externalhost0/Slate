//
// Created by Hayden Rivas on 11/19/24.
//

#include "SettingsWindow.h"

namespace Slate {
    void SettingsWindow::SpawnWindow() {
        if (m_OpenStatus) return;
        m_OpenStatus = true;
        m_Window = glfwCreateWindow(700, 400, "Settings", nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
    }
    void SettingsWindow::OnUpdate(GLFWwindow *pWwindow) {
        if (SettingsWindow::m_OpenStatus) {
            if (!glfwWindowShouldClose(SettingsWindow::GetWindow())) {
                glfwMakeContextCurrent(SettingsWindow::GetWindow());
                glfwPollEvents();
                // main update loop below and before swap

                glfwSwapBuffers(m_Window);
            } else {
                glfwDestroyWindow(SettingsWindow::GetWindow());
                SettingsWindow::m_OpenStatus = false; // allow settings to be reopened
                // return focus to editor window
                glfwMakeContextCurrent(pWwindow);
                glfwFocusWindow(pWwindow);
            }
        }
    }
    GLFWwindow *SettingsWindow::GetWindow() { return m_Window; }
}