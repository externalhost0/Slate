//
// Created by Hayden Rivas on 11/5/24.
//
#ifndef SLATE_EDITORLAYER_H
#define SLATE_EDITORLAYER_H

#include <Slate/Ref.h>
#include <Slate/Renderer.h>
#include <Slate/Scope.h>
#include <utility>

#include "Gui/Framebuffer.h"
#include "Gui/EditorGUI.h"

#include "Context.h"

namespace Slate {
    class EditorGUI;
    struct EditorLayer {
        ~EditorLayer() = default;

        void Bootstrap();
        void OnUpdate();
        void OnImGuiUpdate();
        void OnDetach() const;

        Ref<Context> m_ActiveContext;
        Ref<EditorGUI> m_GUI;
    private:
        Ref<Framebuffer> m_Framebuffer;
    };
}
#endif //SLATE_EDITORLAYER_H
