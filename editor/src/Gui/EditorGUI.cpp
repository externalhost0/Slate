//
// Created by Hayden Rivas on 11/2/24.
//

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>
#include <imoguizmo.hpp>

#include <Slate/Files.h>

#include "Fonts.h"

#include "panels/PropertiesPanel.h"
#include "panels/ScenePanel.h"
#include "panels/ViewportPanel.h"
#include "panels/AssetsPanel.h"

#include "EditorGUI.h"
#include "../Editor.h"
#include "../../external/IconFontCppHeaders/IconsLucide.h"


namespace Slate {


//https://github.com/WerWolv/ImHex/blob/021206e0523dec6a375901fdea76cfe2a7a3b994/lib/libimhex/source/api/imhex_api.cpp#L617
    float getBackingScaleFactor() {
        #if defined(OS_WINDOWS)
            return 1.0F;
        #elif defined(OS_MACOS)
            return 1.0f; // nothing as of now
        #elif defined(OS_LINUX)
            return 1.0F;
        #elif defined(OS_WEB)
            return 1.0F;
        #else
            return 1.0F;
        #endif
    }


    void FontSetup() {
        ImGuiIO &io = ImGui::GetIO();

        // font m_Count controls everything
        float fontSize = 17.f;

        // for crisp text on mac retina displays
        #if defined(OS_MACOS)
            fontSize *= 2.f;
            io.FontGlobalScale = 1.0f / 2.0f;
        #else
            io.FontGlobalScale = 1.0F;
        #endif

        // main font config, for retina displays
        ImFontConfig fontCfg;
        {
            fontCfg.FontDataOwnedByAtlas = false;
            fontCfg.OversampleV = 2;
            fontCfg.OversampleH = 1;
            fontCfg.RasterizerDensity = 1.0f;
            fontCfg.GlyphOffset = ImVec2(-0.4f, 0.0f);
        }
        // icon fonts, how much should we scale the icons
        float iconSize = fontSize * 0.89f;
        ImFontConfig iconFontCfg;
        {
            iconFontCfg.MergeMode = true;
            iconFontCfg.RasterizerDensity = 1.0f; // once again because we are on retina display (mac)
            iconFontCfg.GlyphMinAdvanceX = iconSize; // Use if you want to make the icon monospaced
            iconFontCfg.GlyphOffset = ImVec2(1.0f, fontSize/10.0f); // fixes the offset in the text
            iconFontCfg.PixelSnapH = true;
        }

        static const ImWchar ILC_Range[] = {ICON_MIN_LC, ICON_MAX_16_LC, 0};

        // the path in which all the fonts are located, figure out a better way to set this later
        const std::string path = ToDirectory("assets/fonts/");
        // main font, also merged with below icons
        io.Fonts->AddFontFromFileTTF((path + "NotoSans-Regular.ttf").c_str(), fontSize, &fontCfg);
        io.Fonts->AddFontFromFileTTF((path + FONT_ICON_FILE_NAME_LC).c_str(), iconSize, &iconFontCfg, ILC_Range);

        float mediumSize = 3.0f;
        Fonts::iconMediumFont = io.Fonts->AddFontFromFileTTF((path + "NotoSans-Regular.ttf").c_str(), fontSize + mediumSize, &fontCfg);
        io.Fonts->AddFontFromFileTTF((path + FONT_ICON_FILE_NAME_LC).c_str(), iconSize + mediumSize, &iconFontCfg, ILC_Range);

        float largeSize = 10.0f;
        Fonts::iconLargeFont = io.Fonts->AddFontFromFileTTF((path + "NotoSans-Regular.ttf").c_str(), fontSize + largeSize, &fontCfg);
        io.Fonts->AddFontFromFileTTF((path + FONT_ICON_FILE_NAME_LC).c_str(), iconSize + largeSize, &iconFontCfg, ILC_Range);


        // variants of the NotoSans main font
        // must be after the main font as we merge the fonts with the last font that is added to the io
        Fonts::boldFont = io.Fonts->AddFontFromFileTTF((path + "NotoSans-Bold.ttf").c_str(), fontSize, &fontCfg);
        io.Fonts->AddFontFromFileTTF((path + FONT_ICON_FILE_NAME_LC).c_str(), iconSize, &iconFontCfg, ILC_Range);

        Fonts::largeboldFont = io.Fonts->AddFontFromFileTTF((path + "NotoSans-Bold.ttf").c_str(), fontSize + 5.0f, &fontCfg);
        Fonts::italicFont = io.Fonts->AddFontFromFileTTF((path + "NotoSans-Italic.ttf").c_str(), fontSize, &fontCfg);
        Fonts::largeitalicFont = io.Fonts->AddFontFromFileTTF((path + "NotoSans-Italic.ttf").c_str(), fontSize + 5.0f, &fontCfg);


    }

    void InitStyle() {
        // necessary to be done early
        FontSetup();
        // our default colors for the user interface
        StyleStandard();
    }

    void EditorGUI::OnAttach(const Ref<Context> &context, const Ref<Framebuffer> &framebuffer) {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
        io.ConfigDragClickToInputText = true; // makes single click on sldiers
        io.ConfigWindowsMoveFromTitleBarOnly = true; // makes it so you can only move windows from the bar, required for viewport functionality when undocked
        io.ConfigInputTextCursorBlink = true; // enables blinking cursor in text boxes
#if defined(OS_MACOS)
        io.ConfigMacOSXBehaviors = true; // changes a ton of stuff, just click on it
#endif


        // Setup Platform/RenderManager backends
        ImGui_ImplGlfw_InitForOpenGL(m_EditorWindow.GetNativeWindow(), false);  // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init("#version 410 core");


//        Ref<PropertiesPanel> pp = CreateRef<PropertiesPanel>();

        m_Panels = { CreateRef<PropertiesPanel>(), CreateRef<ViewportPanel>(framebuffer), CreateRef<ScenePanel>(), CreateRef<AssetsPanel>() };

        // apply our style for dear imgui
        InitStyle();


        /// on attach for all panels
        for (auto& panel : m_Panels) {
            panel->LinkContext(context);
            panel->OnAttach();
        }
    }

    void EditorGUI::ActualWindowUpdate() const {
        // everything thats not a dedicated panel
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Scene", "Ctrl+N")) {}
                if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {}
                if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save Scene As", "Ctrl+Shift+S")) {}
                ImGui::Separator();
//                if (ImGui::MenuItem("Settings")) SettingsWindow::SpawnWindow();
//                ImGui::Separator();
                if (ImGui::MenuItem("Exit", "esc")) glfwSetWindowShouldClose(m_EditorWindow.GetNativeWindow(), true);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("BLOW UP", "Ctrl+B")) {}
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();



        // each panel update
        for (auto& panel : m_Panels) {
            panel->OnImGuiUpdate();
        }
    }


    void EditorGUI::PostDrawUpdate() const {
        // necessary setup
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // gizmo frame
        ImGuizmo::BeginFrame();

        // below is all dockspace setup
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoWindowMenuButton; // Config flags for the Dockspace
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

        // Set the parent window's position, m_Count, and viewport to match that of the main viewport. This is so the parent window
        // completely covers the main viewport, giving it a "full-screen" feel.
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        // Manipulate the window flags to make it inaccessible to the user (no titlebar, resize/move, or navigation)
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


        // Set the parent window's styles to match that of the main viewport:
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // No corner rounding on the window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // No border around the window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // remove window padding for "fullscreen" effect

        // opening call to dockspace, matches with postDrawUpdate
        ImGui::Begin("DockSpace Main", nullptr, window_flags);
        ImGui::PopStyleVar(3); // pop previous styles, (padding, rounding, m_Count)

        // check if docking is enabled in io, it NEEDS to be!!:
        // The GetID() function is to give a unique identifier to the Dockspace - here, it's "MyDockSpace".
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        // now let's do the actual stuff below
        ActualWindowUpdate();
        ImGui::End();
    }

    void EditorGUI::OnDetach() const {

        for (auto& panel : m_Panels) {
            panel->OnDetach();
        }
    }

    void EditorGUI::DrawFinish() const {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}