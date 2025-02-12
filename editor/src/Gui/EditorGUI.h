//
// Created by Hayden Rivas on 11/2/24.
//

#pragma once

#include <imgui.h>
#include <utility>

#include <Slate/Application.h>
#include "panels/Panel.h"
#include "Framebuffer.h"
#include "../Context.h"
#include "Slate/Window.h"

namespace Slate {
    class EditorGUI {
    public:
        EditorGUI() = default;
        ~EditorGUI() = default;

        void OnAttach(const Ref<Context> &context, const Ref<Framebuffer> &framebuffer);
        void OnDetach() const;

        void PostDrawUpdate() const;
        void DrawFinish() const;
    private:
        void ActualWindowUpdate() const;

        Ref<Framebuffer> m_Framebuffer;
        Window m_EditorWindow = SystemLocator::Get<WindowManager>().GetWindow();
        std::vector<Ref<Panel>> m_Panels;
    };

    // styles.cpp
    void StyleStandard(ImGuiStyle *dst = nullptr);
}
