cmake_minimum_required(VERSION 3.28)

# slate Options
option(PROJECT_ENABLE_STD_ASSERTS "Enable debug asserts in the C++ std library." ON)

# global cmake vars
set(SLATE_BASE_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}")
set(SLATE_GLOBAL_VERSION "0.0.1")

# cmake settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

# use custom macro/functions
include("${SLATE_BASE_FOLDER}/cmake/buildhelpers.cmake")

# coupled project
project(Slate
        LANGUAGES C CXX
        VERSION ${SLATE_GLOBAL_VERSION}
        DESCRIPTION "Slate Editor & Engine"
)

# add defines
helper_detectOS()
helper_addDefines()

# Slate Engine & Editor
add_subdirectory(engine)
add_subdirectory(editor)