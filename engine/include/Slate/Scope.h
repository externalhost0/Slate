//
// Created by Hayden Rivas on 11/26/24.
//

#ifndef SLATEEDITOR_SCOPE_H
#define SLATEEDITOR_SCOPE_H
#include <memory>
namespace Slate {
    // alias for std::shared_ptr
    template<typename T>
    using Scope = std::unique_ptr<T>;

    // for populating the shared_ptr
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args &&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}

#endif //SLATEEDITOR_SCOPE_H
