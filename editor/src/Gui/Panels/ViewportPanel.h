//
// Created by Hayden Rivas on 11/10/24.
//

#ifndef SLATE_VIEWPORTPANEL_H
#define SLATE_VIEWPORTPANEL_H

#include "imgui.h"
#include "ImGuizmo.h"

#include "Panel.h"

#include "../../ViewportCamera.h"
#include "../Framebuffer.h"

namespace Slate {
    enum class ViewportModes {
        SHADED,
        UNSHADED,
        DEPTH,
        NORMALS,
        OVERDRAW,
        WIREFRAME,
        SHADED_WIREFRAME,
    };
    class ViewportPanel : public Panel {
    public:
        explicit ViewportPanel(Ref<Framebuffer> framebuffer) : m_Framebuffer(std::move(framebuffer)) {}

        ImGuizmo::OPERATION m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
        ImGuizmo::MODE m_GuizmoSpace = ImGuizmo::MODE::WORLD;


        Ref<ViewportCamera> m_ViewportCamera;
        glm::vec2 m_ViewportSize{ 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2]{};

    private:
        bool IsInViewportBounds();
        Ref<Framebuffer> m_Framebuffer;
        Ref<Framebuffer> m_PostProcessFramebuffer;
        ViewportModes m_ViewportMode = ViewportModes::SHADED;
        Entity m_HoveredEntity;
        void OnAttach() override;
        void OnImGuiUpdate() override;
        void OnDetach() override;
        void IndependentInput() override;
    };
}

#endif //SLATE_VIEWPORTPANEL_H
