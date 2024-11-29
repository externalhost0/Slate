//
// Created by Hayden Rivas on 11/3/24.
//

#include <imgui.h>
#include <ImGuizmo.h>

#include "EditorGUI.h"

namespace Slate {

    struct ColorTheme {
        ImVec4 primary;
        ImVec4 secondary;
        ImVec4 tertiary;
    };


    void StyleStandard(ImGuiStyle *dst) {
        ImGui::StyleColorsDark();
        ImGuiStyle *style = dst ? dst : &ImGui::GetStyle();
        {
            style->DockingSeparatorSize = 2.0f;
            style->FrameBorderSize = 1.0f;
            style->FramePadding = ImVec2(10.0f, 4.0f);
            style->TabBarBorderSize = 2.0f;
            style->TabBarOverlineSize = 1.0f;
            style->WindowBorderSize = 1.0f; // for the thick borders on everything
            style->PopupBorderSize = 1.0f; // for those nice borders around things like menus
            style->ScrollbarSize = 10.0f;

            style->WindowRounding = 2.0f;
            style->ScrollbarRounding = 0.0f;
            style->GrabRounding = 1.0f;
            style->FrameRounding = 1.0f;
            style->ChildRounding = 2.0f;
        }
        ImVec4 *colors = style->Colors;
        {
            colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
            colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
            colors[ImGuiCol_ChildBg] = ImVec4(0.50f, 0.50f, 0.50f, 0.10f);
            colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.0f);

            colors[ImGuiCol_Border] = ImVec4(0.39f, 0.39f, 0.35f, 0.50f);
            colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.28f);

            colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
            colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.84f);
            colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);

            colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

            colors[ImGuiCol_MenuBarBg] = colors[ImGuiCol_WindowBg];

            // lesser colors
            colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
            colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
            colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
            colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
            colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
            colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
            colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
            colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
            colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
            colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);

            colors[ImGuiCol_Header] = ImVec4(0.03f, 0.03f, 0.03f, 0.72f);
            colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.22f, 0.23f, 0.3f);
            colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.53f);

            colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.49f);
            colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
            colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
            colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
            colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
            colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
            colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
            colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
            colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
            colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);

            colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

            // HIGHLIGHT COLORS
            // colors for docking feature on hover
            colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.10f, 0.10f, 0.10f, 0.70f);
            colors[ImGuiCol_DockingPreview] = ImVec4(0.10f, 1.00f, 0.60f, 0.40f);
            colors[ImGuiCol_DragDropTarget] = ImVec4(0.10f, 1.0f, 0.60f, 1.00f);

            colors[ImGuiCol_CheckMark] = ImVec4(0.10f, 1.0f, 0.60f, 1.00f);

            colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.10f, 1.0f, 0.60f, 1.00f);


        }
        ImGuizmo::Style* guizmostyle = &ImGuizmo::GetStyle();
        float thickness = 4.0f;
        {
            guizmostyle->TranslationLineThickness = thickness;
            guizmostyle->RotationLineThickness = thickness - 2.0f;
            guizmostyle->ScaleLineThickness = thickness;
            guizmostyle->TranslationLineArrowSize = 7.0f;
            guizmostyle->RotationOuterLineThickness = thickness - 1.0f;
            guizmostyle->HatchedAxisLineThickness = 1.0f;
        }

        ImVec4* guizmocolors = guizmostyle->Colors;
        {
            guizmocolors[ImGuizmo::DIRECTION_X] = ImVec4(0.858f, 0.243f, 0.113f, 0.929f);
            guizmocolors[ImGuizmo::DIRECTION_Y] = ImVec4(0.603f, 0.952f, 0.282f, 0.929f);
            guizmocolors[ImGuizmo::DIRECTION_Z] = ImVec4(0.227f, 0.478f, 0.972f, 0.929f);
            guizmocolors[ImGuizmo::ROTATION_USING_FILL] =  ImVec4(0.727f, 0.478f, 0.072f, 1.0f);
            guizmocolors[ImGuizmo::ROTATION_USING_BORDER] = ImVec4(0.5f, 0.3f, 0.05f, 0.8f);
        }
    }
}