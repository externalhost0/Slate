//
// Created by Hayden Rivas on 11/2/24.
//
// https://www.youtube.com/watch?v=F6_WdnzQIQ4&list=PLv8Ddw9K0JPg1BEO-RS-0MYs423cvLVtj&index=3

#ifndef SLATE_PLATFORM_H
#define SLATE_PLATFORM_H

namespace Slate {
// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif

#elif defined(__linux__) || defined(__gnu_linux__)
#define PLATFORM_LINUX 1
#if defined(__ANDROID__)
#define PLATFORM_ANDROID 1
#endif

#elif defined(__unix__)
#define PLATFORM_UNIX 1

#elif defined(_POSIX_VERSION)
#define PLATFORM_POSIX 1

#elif defined(__APPLE__)
#define PLATFORM_APPLE 1

#else
#error "Unknown platform!"
#endif

}
#endif //SLATE_PLATFORM_H
