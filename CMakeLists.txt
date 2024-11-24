cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME "SlateEditor")
set(VERSION "0.0.1")

project(${PROJECT_NAME} VERSION ${VERSION} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(engine)

add_executable(${PROJECT_NAME}

        editor/src/main.cpp
        editor/src/Editor.cpp
        editor/src/EditorLayer.cpp
        editor/src/EditorWindow.cpp
        editor/src/SettingsWindow.cpp

        editor/src/GUI/EditorGUI.cpp
        editor/src/GUI/Framebuffer.cpp
        editor/src/GUI/ImGuiStyles.cpp
        editor/src/GUI/ViewportCamera.cpp
        editor/src/GUI/ImGuiSnippets.cpp

        editor/src/GUI/Panels/AssetsPanel.cpp
        editor/src/GUI/Panels/PropertiesPanel.cpp
        editor/src/GUI/Panels/ScenePanel.cpp
        editor/src/GUI/Panels/ViewportPanel.cpp
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
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/license.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
include(CPack)
