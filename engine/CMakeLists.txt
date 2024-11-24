cmake_minimum_required(VERSION 3.28)

# dev defined properties
set(PROJECT_NAME "SlateEngine")
set(VERSION "0.0.1")

project(${PROJECT_NAME} VERSION ${VERSION} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# source files
add_library(${PROJECT_NAME}
        src/Time.cpp
        src/Input.cpp
        src/Renderer.cpp
        src/Scene.cpp
        src/Files.cpp
        src/Mesh.cpp
        src/Shader.cpp

        src/ECS/ModelComponent.cpp
        src/ECS/Entity.cpp
        src/ECS/UUID.cpp
)


# packages
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)


# include and link it
target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
            glm::glm
            glfw
            EnTT::EnTT
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
            glm::glm
            glfw
            GLEW::GLEW
            EnTT::EnTT
)
