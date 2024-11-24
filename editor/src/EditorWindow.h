//
// Created by Hayden Rivas on 10/30/24.
//

#ifndef SLATE_EDITORWINDOW_H
#define SLATE_EDITORWINDOW_H


#include "GLFW/glfw3.h"
#include <string>

#include "Slate/Types.h"

namespace Slate {
    class EditorWindow {
    public:
        EditorWindow() = default;
        ~EditorWindow() = default;

        bool CreateWindow();
        void SetVideoMode(VIDEO_MODE mode);
        void SetVsync(int refresh);

        GLFWwindow* GetWindow();
    private:
        GLFWmonitor *m_WindowMonitor{};
        GLFWwindow* m_Window{};
        VIDEO_MODE m_VideoMode{WINDOWED};
        uint16_t m_WindowWidth{1300}, m_WindowHeight{720};

        // more config stuffs
        std::string m_WindowTitle{"Slate Editor"};
        bool m_IsResizeable{true};
        int m_RefreshRate{};
        uint8_t m_VSync{0}; // 0 means no wait (disabled), 1 means one screen wait (enabled)

    };
}
#endif //SLATE_EDITORWINDOW_H
