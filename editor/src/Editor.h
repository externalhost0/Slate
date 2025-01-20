//
// Created by Hayden Rivas on 10/30/24.
//
#pragma once
// things the editor requires
// 1. a window
// 2. a gui
// any other essentials will be relocated
#include <Slate/Scope.h>
#include <Slate/Application.h>

#include "Context.h"
#include "gui/EditorGUI.h"

namespace Slate {
    // inherit publicly to use create function!!
    class Editor : public Application {
    public:
        Ref<Context> m_ActiveContext;
        Ref<EditorGUI> m_GUI;

    private:
        // processing of ediotr steps
        // we override the applications events given to us
        void Initialize() override;
        void Loop() override;
        void Shutdown() override;
    private:
        Ref<Framebuffer> m_Framebuffer;
    };
}