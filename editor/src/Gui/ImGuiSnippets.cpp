//
// Created by Hayden Rivas on 11/13/24.
//

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

#include "ImGuiSnippets.h"

namespace Slate {
    ImVec4 Brighten(const ImVec4& color, float amount) {
        // clamp between 0 and 1
        amount = std::clamp(amount, 0.0f, 1.0f);
        // adjust each channel
        return {
                std::min(color.x + amount, 1.0f),
                std::min(color.y + amount, 1.0f),
                std::min(color.z + amount, 1.0f),
                color.w
        };
    }
    ImVec4 Desaturate(const ImVec4& color, float amount) {
        // percieved luminance
        float grayscale = color.x * 0.299f + color.y * 0.587f + color.z * 0.114f;
        // lerp between original and graysscale
        float r = color.x + amount * (grayscale - color.x);
        float g = color.y + amount * (grayscale - color.y);
        float b = color.z + amount * (grayscale - color.z);
        // return the new ImVec4 with the adjusted RGB and original alpha
        return {r, g, b, color.w};
    }

    void Vector3Drag(const char *label, glm::vec3 &value, float columnWidth, float resetValue, float dragSpeed) {
        ImGui::PushID(label);

        ImGui::Columns(2);
        // just the space for the text that is represented below
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label);
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{1, 0}); // spacing between the X, Y, Z and the actual values
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f); // the border around the buttons

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = {ImGui::CalcTextSize("X").x*2, lineHeight};
        ImVec2 framePadding = { ImGui::CalcTextSize("X").x/2, 0.0f };

        ImGuizmo::Style guizmostyle  = ImGuizmo::GetStyle();
        ImVec4* guizmocolors = guizmostyle.Colors;
        // float 3 drawing below
        ImGui::PushStyleColor(ImGuiCol_Button, Desaturate(guizmocolors[ImGuizmo::DIRECTION_X], -0.15f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Desaturate(guizmocolors[ImGuizmo::DIRECTION_X], 0.25f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, guizmocolors[ImGuizmo::DIRECTION_X]);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, framePadding);
        if (ImGui::Button("X", buttonSize)) value.x = resetValue;
        ImGui::PopStyleVar();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &value.x, dragSpeed);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, Desaturate(guizmocolors[ImGuizmo::DIRECTION_Y], -0.15f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Desaturate(guizmocolors[ImGuizmo::DIRECTION_Y], 0.45f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, guizmocolors[ImGuizmo::DIRECTION_Y]);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, framePadding);
        if (ImGui::Button("Y", buttonSize)) value.y = resetValue;
        ImGui::PopStyleVar();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &value.y, dragSpeed);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, Desaturate(guizmocolors[ImGuizmo::DIRECTION_Z], -0.15f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Desaturate(guizmocolors[ImGuizmo::DIRECTION_Z], 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, guizmocolors[ImGuizmo::DIRECTION_Z]);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, framePadding);
        if (ImGui::Button("Z", buttonSize)) value.z = resetValue;
        ImGui::PopStyleVar();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &value.z, dragSpeed);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopStyleVar();

        ImGui::PopID();
    }
    void HighlightedText(const char* text, ImVec4 bg_color, ImVec2 padding, ImVec4 text_color) {
        ImGui::PushStyleColor(ImGuiCol_Text, text_color);
        ImVec2 text_size = ImGui::CalcTextSize(text);
        ImVec2 cursor_pos = ImGui::GetCursorScreenPos();

        // Add padding around the text
        float rect_min_x = cursor_pos.x - padding.x;
        float rect_min_y = cursor_pos.y - padding.y;
        float rect_max_x = cursor_pos.x + text_size.x + padding.x;
        float rect_max_y = cursor_pos.y + text_size.y + padding.y;

        // Draw the background rectangle
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(
                ImVec2(rect_min_x, rect_min_y),
                ImVec2(rect_max_x, rect_max_y),
                ImGui::GetColorU32(bg_color),
                0.0f // Rounded corners (optional)
        );

        ImGui::TextUnformatted(text);
        ImGui::PopStyleColor();
    }
    glm::vec2 WorldToScreen(const glm::vec3& worldPos, const glm::mat4& view, const glm::mat4& projection, float screenWidth, float screenHeight) {
        glm::vec4 clipSpacePos = projection * view * glm::vec4(worldPos, 1.0f);
        if (clipSpacePos.w <= 0.0f) {
            return {-1.0f, -1.0f}; // if outside frustum
        }

        glm::vec3 ndc = glm::vec3(clipSpacePos) / clipSpacePos.w;
        glm::vec2 screenPos = glm::vec2(
                (ndc.x * 0.5f + 0.5f) * screenWidth,
                (1.0f - (ndc.y * 0.5f + 0.5f)) * screenHeight
        );
        return screenPos;
    }
    void Render3DText(const std::string& text, const glm::vec3& worldPos, const glm::vec3& cameraPos, const glm::mat4& view, const glm::mat4& projection, float screenWidth, float screenHeight) {
        Render3DText(text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), worldPos, cameraPos, view, projection, screenWidth, screenHeight);
    }
    void Render3DText(const std::string& text, const ImVec4& color, const glm::vec3& worldPos, const glm::vec3& cameraPos, const glm::mat4& view, const glm::mat4& projection, float screenWidth, float screenHeight) {
        float distance = glm::length(worldPos - cameraPos);
        // just stop rendering if we are too far
        if (distance > 90.0f) return;

        glm::vec2 screenPos = WorldToScreen(worldPos, view, projection, screenWidth, screenHeight);
        // also stop if outside frustum
        if (screenPos.x == -1.0f && screenPos.y == -1.0f) return;
//        std::cout << "World: " << screenPos.x << ", " << screenPos.y << std::endl;

        // stop if transform point is outside bounds
        // we could make this better if we used the calculated text size but its just kinda overkill
        float padding = 10.0f;
        if (screenPos.x < 0-padding || screenPos.y < 0-padding || screenPos.x > screenWidth+padding || screenPos.y > screenHeight+padding)
            return;

        // scaling based on distance
        float baseSize = 23.0f;
        float minScale = 0.5f;
        float maxScale = 5.0f;
        float scaleFactor = baseSize / (distance + 3.0f);
        scaleFactor = glm::clamp(scaleFactor, minScale, maxScale);
        ImFont* defaultFont = ImGui::GetDefaultFont();
        float scaledFontSize = (baseSize * (baseSize / defaultFont->FontSize)) * scaleFactor;

        // FIXME: needs its y axis corrected, currently looks off when backing up!
        // correction based on new scale to center
        ImVec2 textSize = defaultFont->CalcTextSizeA(scaledFontSize, FLT_MAX, 0.0f, text.c_str());
        ImVec2 adjustedPos = ImVec2(screenPos.x - (textSize.x / 2), screenPos.y + (textSize.y * 1/scaleFactor)/scaleFactor);
//        std::cout << "Adjusted: " << adjustedPos.x << ", " << adjustedPos.y << std::endl;

        // now draw it on top of image
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddText(defaultFont, scaledFontSize, adjustedPos, ImGui::ColorConvertFloat4ToU32(color), text.c_str());
    }

}