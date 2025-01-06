//
// Created by Hayden Rivas on 10/30/24.
//
#pragma once

#include <cstdlib>
#include <cstdio>
#include <string>
#include "IManager.h"

namespace Slate {
    class FileManager : public IManager {
    private:
        void Initialize() override {};
        void Shutdown() override {};
        friend class Application;
    private:
    };

    std::string loadCurrentDirectoryLocation();
    std::string ToDirectory(const std::string& path);
}
