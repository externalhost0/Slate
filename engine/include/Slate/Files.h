//
// Created by Hayden Rivas on 10/30/24.
//
#ifndef SLATE_FILES_H
#define SLATE_FILES_H

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
}

#endif //SLATE_FILES_H