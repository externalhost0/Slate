//
// Created by Hayden Rivas on 10/29/24.
//
#include "Slate/Files.h"
#include <string>

namespace Slate {

    [[maybe_unused]]
    std::string loadCurrentDirectoryLocation() {
        return "../../editor/";
    }
    [[maybe_unused]]
    std::string ToDirectory(const std::string& filepath) {
        return loadCurrentDirectoryLocation() + filepath;
    }


}