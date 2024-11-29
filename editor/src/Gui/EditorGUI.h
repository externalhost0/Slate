//
// Created by Hayden Rivas on 11/2/24.
//

#ifndef SLATE_GUI_H
#define SLATE_GUI_H

#include <imgui.h>
#include <utility>

#include "Panels/Panel.h"
#include "Framebuffer.h"
#include "../Window/Window.h"
#include "../Context.h"

namespace Slate {
    class EditorGUI {
    public:
        EditorGUI();
        ~EditorGUI() = default;

        void OnAttach(const Ref<Context> &context, const Ref<Framebuffer> &framebuffer);
        void OnDetach() const;

        void PostDrawUpdate() const;
        void DrawFinish() const;
    private:
        void ActualWindowUpdate() const;

        Ref<Framebuffer> m_Framebuffer;
        Window* m_EditorWindow;
        std::vector<Ref<Panel>> m_Panels;
    };

    // styles.cpp
    void StyleStandard(ImGuiStyle *dst = nullptr);
}
#endif //SLATE_GUI_H
