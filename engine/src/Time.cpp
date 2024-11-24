//
// Created by Hayden Rivas on 11/11/24.
//

#include "Slate/Time.h"

namespace Slate {
    void Time::CalculateDeltaTime() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta = currentTime - lastTime;
        lastTime = currentTime;
        deltaTime = delta.count();
    }

    double Time::GetDeltaTime() { return deltaTime; }
}