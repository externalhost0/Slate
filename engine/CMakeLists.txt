cmake_minimum_required(VERSION 3.28)

# dev defined properties
set(PROJECT_NAME "SlateEngine")
set(VERSION "0.0.1")

project(${PROJECT_NAME} VERSION ${VERSION} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# source files
add_library(${PROJECT_NAME}
        lib/Application.cpp
        lib/Time.cpp
        lib/Input.cpp
        lib/Render.cpp
        lib/Scene.cpp
        lib/Files.cpp
        lib/Mesh.cpp
        lib/Shader.cpp

        lib/Entity.cpp
        lib/MeshImport.cpp
        lib/UUID.cpp
        lib/ShaderLibrary.cpp
        lib/Font.cpp
        lib/Window.cpp
)

# packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(Stb REQUIRED)


# include and link it
target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
            ${Stb_INCLUDE_DIR}
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
            glm::glm
            glfw
            glad::glad
            EnTT::EnTT
            Freetype::Freetype
)
