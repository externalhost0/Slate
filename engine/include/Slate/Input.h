//
// Created by Hayden Rivas on 10/30/24.
//

#ifndef SLATE_INPUT_H
#define SLATE_INPUT_H

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

namespace Slate {
    class Input {
    public:
        static bool IsKeyPressed(int key);
        static bool IsMouseButtonPressed(int button);
        static glm::vec2 GetMousePosition();

        static void SetInputMode(int mode);
        static int GetInputMode();
        // inject window variable
        static void SetWindow(GLFWwindow& iWindow);
    private:
        inline static GLFWwindow* m_Window;
    };

}
#endif //SLATE_INPUT_H
