//
// Created by Hayden Rivas on 11/25/24.
//

#ifndef SLATEEDITOR_EXPECT_H
#define SLATEEDITOR_EXPECT_H
#include <csignal>
#include <functional>

namespace Slate {
    // our error macro
#define EXPECT(ERROR, FORMAT, ...) {                                                                                                        \
    int macroErrorCode = ERROR;                                                                                                             \
    if(!macroErrorCode) {                                                                                                                    \
        fprintf(stderr, "%s -> %s -> %i -> Error(%i):\n\t" FORMAT "\n", __FILE__, __func__, __LINE__, macroErrorCode, ##__VA_ARGS__);       \
        raise(SIGABRT);                                                                                                                     \
    }                                                                                                                                       \
}
}
#endif //SLATEEDITOR_EXPECT_H
