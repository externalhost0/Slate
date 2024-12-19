//
// Created by Hayden Rivas on 11/13/24.
//

#ifndef SLATEEDITOR_IMGUISNIPPETS_H
#define SLATEEDITOR_IMGUISNIPPETS_H

#include <glm/glm.hpp>
#include <functional>
#include <utility>
#include <Slate/Entity.h>

#include "../../external/IconFontCppHeaders/IconsLucide.h"
#include "Fonts.h"

namespace Slate {
    struct TransformComponent;
    // color helper functions
    ImVec4 Brighten(const ImVec4& color, float amount);
    ImVec4 Desaturate(const ImVec4& color, float amount);
    ImVec4 HueShift(const ImVec4& color, float amount);


    // short imgui widhets
    void Vector3Drag(const char* label, glm::vec3 &value, float columnWidth, float resetValue, float dragSpeed);
    void HighlightedText(const char* text, ImVec4 bg_color, ImVec2 padding = {0.0f, 0.0f}, ImVec4 text_color = {1, 1, 1, 1});

    void Render3DText(const std::string& text, const ImVec4& color, const glm::vec3& worldPos, const glm::vec3& cameraPos, const glm::mat4& view, const glm::mat4& projection, float screenWidth, float screenHeight);
    void Render3DText(const std::string& text, const glm::vec3& worldPos, const glm::vec3& cameraPos, const glm::mat4& view, const glm::mat4& projection, float screenWidth, float screenHeight);

    // templated function for all panels
    template <typename ComponentType, typename Func>
    void ComponentPropertyPanel(Entity* entity, Func func, const char* title, const char* icon) {
        // make sure a component is only shown if an entity has it
        if (!entity || !entity->HasComponent<ComponentType>()) return;
        // require that the componenttype is actually a component lol
        static_assert(std::is_class<ComponentType>::value, "ComponentType must be a class/struct!");

        // some flags we want for the following imgui elements
        ImGuiChildFlags childFlags = ImGuiChildFlags_AutoResizeY;
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap;

        // generate a unique id for the child window
        std::string uniqueId = "Child" + std::to_string(typeid(ComponentType).hash_code()) + std::to_string(entity->operator uint32_t());
        ImGui::BeginChild(uniqueId.c_str(), ImVec2(0, 0), childFlags);

        // this constructs the icon -> title -> and set tree visibillity
        ImGui::PushFont(Fonts::boldFont);
        float cursorY = ImGui::GetCursorPosY();
        bool isOpen = ImGui::TreeNodeEx((void*) typeid(ComponentType).hash_code(), nodeFlags, "%s", icon);
        ImGui::SetCursorPosY(cursorY);
        float pad = ImGui::GetFontSize() * 4.2f;
        ImGui::SameLine(pad);
        ImGui::Text("%s", title);
        ImGui::PopFont();

        bool remove = false;
        const char* label = ICON_LC_TRASH;
        float space_available = ImGui::GetContentRegionAvail().x;
        if (space_available > ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x * 2) {
           ImGui::SameLine(ImGui::GetCursorPosX() + space_available - ImGui::CalcTextSize(label).x - ImGui::GetStyle().FramePadding.x * 2);

           ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.961, 0.357, 0.357, 0.7f));
           if (ImGui::Button(label)) {
               remove = true;
           }
           ImGui::PopStyleColor();
        }
        // where your passed function will go
        if (isOpen){
            func(entity); // run the custom property gui you gave
            ImGui::TreePop();
        }
        // remove at end of call
        if (remove) {
            entity->RemoveComponent<ComponentType>();
            if (std::is_same_v<ComponentType, TransformComponent>) {
                entity = nullptr;
            }
        }
        ImGui::EndChild();
    }
}


#endif //SLATEEDITOR_IMGUISNIPPETS_H
