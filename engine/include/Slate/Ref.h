//
// Created by Hayden Rivas on 11/11/24.
//

#pragma once

#include <memory>

namespace Slate {
    // alias for std::shared_ptr
    template<typename T>
    using Ref = std::shared_ptr<T>;

    // for populating the shared_ptr
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args &&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
