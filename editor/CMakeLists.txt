# project settings
set(PROJECT_NAME "SlateEditor")
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Editor.cpp
        src/ViewportCamera.cpp

        src/gui/EditorGUI.cpp
        src/gui/Framebuffer.cpp
        src/gui/ImGuiStyles.cpp
        src/gui/ImGuiSnippets.cpp

        src/gui/panels/AssetsPanel.cpp
        src/gui/panels/PropertiesPanel.cpp
        src/gui/panels/ScenePanel.cpp
        src/gui/panels/ViewportPanel.cpp
)

find_package(imgui CONFIG REQUIRED)
find_package(ImGuizmo CONFIG REQUIRED)
find_package(nfd CONFIG REQUIRED)
# git submodules
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/imoguizmo)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        SlateEngine
        imgui::imgui
        imguizmo::imguizmo
        imoguizmo
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        SlateEngine
        imgui::imgui
        imguizmo::imguizmo
        nfd::nfd
)
