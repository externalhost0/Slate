cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME "SlateEditor")
set(VERSION "0.0.1")

project(${PROJECT_NAME} VERSION ${VERSION} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(engine)

add_executable(${PROJECT_NAME}
        editor/src/main.cpp
        editor/src/Editor.cpp
        editor/src/ViewportCamera.cpp

        editor/src/Gui/EditorGUI.cpp
        editor/src/Gui/Framebuffer.cpp
        editor/src/Gui/ImGuiStyles.cpp
        editor/src/Gui/ImGuiSnippets.cpp

        editor/src/Gui/Panels/AssetsPanel.cpp
        editor/src/Gui/Panels/PropertiesPanel.cpp
        editor/src/Gui/Panels/ScenePanel.cpp
        editor/src/Gui/Panels/ViewportPanel.cpp
)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGuizmo CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
# git submodules
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/editor/external/imoguizmo)


target_include_directories(${PROJECT_NAME}
        PRIVATE
            SlateEngine
            imgui::imgui
            imguizmo::imguizmo
            imoguizmo
            assimp::assimp
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            SlateEngine
            imgui::imgui
            imguizmo::imguizmo
            yaml-cpp::yaml-cpp
            assimp::assimp
)

# packing setup
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Hayden Rivas")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
include(CPack)
