//
// Created by Hayden Rivas on 11/2/24.
//
// https://www.youtube.com/watch?v=F6_WdnzQIQ4&list=PLv8Ddw9K0JPg1BEO-RS-0MYs423cvLVtj&index=3

#ifndef SLATE_DEFINES_H
#define SLATE_DEFINES_H

//#include <memory>
// below for expect
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

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define KPLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif

#elif defined(__linux__) || defined(__gnu_linux__)
#define KPLATFORM_LINUX 1
#if defined(__ANDROID__)
#define KPLATFORM_ANDROID 1
#endif

#elif defined(__unix__)
#define KPLATFORM_UNIX 1

#elif defined(_POSIX_VERSION)
#define KPLATFORM_POSIX 1

#elif defined(__APPLE__)
#define KPLATFORM_APPLE 1

#include <TargetConditionals.h>

// Debugging output
#if defined(TARGET_IPHONE_SIMULATOR)
#define KPLATFORM_IOS 1
#define KPLATFORM_IOS_SIMULATOR 1
#elif defined(TARGET_OS_IPHONE)
#define KPLATFORM_IOS 1
    #elif defined(TARGET_OS_MAC)
        #define KPLATFORM_MACOS 1
    #else
        #error "Unknown Apple platform"
#endif
#else
#error "Unknown platform!"
#endif

}
#endif //SLATE_DEFINES_H
