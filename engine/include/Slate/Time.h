//
// Created by Hayden Rivas on 11/11/24.
//

#ifndef SLATE_TIME_H
#define SLATE_TIME_H

#include <chrono>

namespace Slate {
    class Time {
    public:
        static void CalculateDeltaTime();
        static double GetDeltaTime();
    private:
        inline static double deltaTime;
        inline static std::chrono::high_resolution_clock::time_point lastTime;
    };
}

#endif //SLATE_TIME_H
