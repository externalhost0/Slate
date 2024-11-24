//
// Created by Hayden Rivas on 11/5/24.
//

#ifndef SLATE_EDITORLAYER_H
#define SLATE_EDITORLAYER_H


#include <Slate/Ref.h>
#include <Slate/Renderer.h>
#include <utility>

#include "GUI/Framebuffer.h"
#include "GUI/EditorGUI.h"
#include "Context.h"

namespace Slate {
    class EditorGUI;

    struct EditorLayer {

        explicit EditorLayer(Ref<EditorWindow> window) : m_EditorWindow(std::move(window)) {}
        ~EditorLayer() = default;

        void Bootstrap();
        void OnUpdate();
        void OnDetach() const;

        EditorGUI* m_GUI{};

        Ref<Renderer> m_Renderer;
        Ref<Context> m_ActiveContext;
    private:
        Ref<Framebuffer> m_Framebuffer;
        Ref<EditorWindow> m_EditorWindow;
    };
}
#endif //SLATE_EDITORLAYER_H
