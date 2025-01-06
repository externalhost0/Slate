set(PROJECT_NAME "SlateEngine")
project(${PROJECT_NAME})

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
        include/Slate/MeshGenerators.h
        lib/Math.cpp
        include/Slate/Math.h
)

# packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

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
            assimp::assimp
            Freetype::Freetype
)
