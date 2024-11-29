//
// Created by Hayden Rivas on 11/26/24.
//

#ifndef SLATEEDITOR_WINDOW_H
#define SLATEEDITOR_WINDOW_H

#include <GLFW/glfw3.h>
#include <Slate/Enums.h>

#include <string>
#include <utility>

namespace Slate {
    // default window settings
    struct WindowSpecification {
        bool m_IsResizeable = true;
        bool m_VSync = false;
        unsigned int m_RefreshRate = 60;
        std::string m_WindowTitle = "Unnamed Window";
        VIDEO_MODE m_VideoMode = VIDEO_MODE::WINDOWED;
        unsigned int m_WindowWidth = 1280, m_WindowHeight = 720;
    };
    class Window {
    public:
        // required for polymorphism
        virtual ~Window() = default;
        // default specification
        Window() : Window(WindowSpecification{}) {}
        // custom specification
        explicit Window(WindowSpecification spec)
        : m_WindowSpecification(std::move(spec)) {
            BuildWindow();
        };

        // system
        void OnWindowAttach() {

        };
        void OnWindowUpdate() {

        };
        void OnWindowDetach() {

        };

        // getters
        GLFWwindow* GetNativeWindow() const { return m_NativeWindow; };
        const WindowSpecification &GetSpecification() const { return m_WindowSpecification; };
    private:
        void BuildWindow();
        WindowSpecification m_WindowSpecification;
        GLFWwindow* m_NativeWindow{nullptr};
    };
}

#endif //SLATEEDITOR_WINDOW_H
