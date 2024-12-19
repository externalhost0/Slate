//
// Created by Hayden Rivas on 10/30/24.
//

#ifndef SLATE_INPUT_H
#define SLATE_INPUT_H

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include "IManager.h"

namespace Slate {
    class InputManager : public IManager {
    public:

        bool IsKeyPressed(int key);
        bool IsMouseButtonPressed(int button);
        glm::vec2 GetMousePosition();

        void SetInputMode(int mode);
        int GetInputMode();
    private:
        void Initialize() override {};
        void Shutdown() override {};
        friend class Application;
    private:
    };

}
#endif //SLATE_INPUT_H
