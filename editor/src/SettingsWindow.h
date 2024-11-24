//
// Created by Hayden Rivas on 11/19/24.
//

#ifndef SLATEEDITOR_SETTINGSWINDOW_H
#define SLATEEDITOR_SETTINGSWINDOW_H

#include <GLFW/glfw3.h>

namespace Slate {
    class SettingsWindow {
    public:
        static void SpawnWindow();
        static void OnUpdate(GLFWwindow *pWwindow);
        static inline bool m_OpenStatus{false};

        static GLFWwindow *GetWindow();

    private:
        static inline GLFWwindow* m_Window;
    };
}

#endif //SLATEEDITOR_SETTINGSWINDOW_H
