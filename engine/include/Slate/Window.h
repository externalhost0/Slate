//
// Created by Hayden Rivas on 12/7/24.
//

#pragma once

#include "IManager.h"
#include "Enums.h"

#include <GLFW/glfw3.h>
#include <string>

namespace Slate {
    // how a window is defined and its properties
    struct WindowSpecification {
        bool m_IsResizeable = true;
        bool m_VSync = false;

        unsigned int m_RefreshRate = 60;
        unsigned int m_WindowWidth = 1280, m_WindowHeight = 720;

        std::string m_WindowTitle = "Unnamed Window";
        Slate::VIDEO_MODE m_VideoMode = VIDEO_MODE::WINDOWED;
    };
    // because we can have multiple windows, its important we have it be given to the single WindowManager
    class Window {
    public:
        Window() = default;
        // preffered constructor in place
        explicit Window(WindowSpecification windowSpecification)
        : m_WindowSpecification(std::move(windowSpecification)) {}
        ~Window() = default;
    public:
        void ConstructWindow();
        void DestroyWindow();

        GLFWwindow* GetNativeWindow() const { return m_NativeWindow; };
        const WindowSpecification &GetSpecification() const { return m_WindowSpecification; };

    private:
        WindowSpecification m_WindowSpecification;
        GLFWwindow* m_NativeWindow{nullptr};
    };

    class WindowManager : public IManager {
    public:
        Window& GetWindow() { return m_Window; }
        void SetMainWindow(const Window& window) { m_Window = window; }

    private:
        void Initialize() override {};
        void Shutdown() override {};
        friend class Application;
    private:
        Window m_Window;
    };

}