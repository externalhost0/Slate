//
// Created by Hayden Rivas on 11/2/24.
//

#ifndef SLATE_GUI_H
#define SLATE_GUI_H

#include <imgui.h>
#include <utility>

#include "Panels/Panel.h"
#include "Framebuffer.h"
#include "../EditorWindow.h"
#include "../Context.h"

namespace Slate {
    class EditorGUI {
    public:

        explicit EditorGUI(Ref<EditorWindow> window) : m_EditorWindow(std::move(window)) {}
        ~EditorGUI() = default;

        void OnAttach(const Ref<Context> &context, const Ref<Framebuffer> &framebuffer);
        void OnUpdate() const;
        void ActualWindowUpdate() const;
        void DrawFinish() const;
        void OnDetach();


    private:
        Ref<Framebuffer> m_Framebuffer;
        Ref<EditorWindow> m_EditorWindow;
        std::vector<Ref<Panel>> m_Panels;
    };

    // styles.cpp
    void StyleStandard(ImGuiStyle *dst = nullptr);
}
#endif //SLATE_GUI_H
