//
// Created by Hayden Rivas on 10/30/24.
//

#ifndef SLATE_EDITOR_H
#define SLATE_EDITOR_H

// things the editor requires
// 1. a window
// 2. a gui
// any other essentials will be relocated
#include <yaml-cpp/yaml.h>
#include "EditorWindow.h"
#include "EditorLayer.h"

namespace Slate {
    class Editor {
    private:
        // processing of engine steps
        bool Init();
        void Loop();
        void Shutdown();
    public:
        // constructors & destructors
        // obviously required and window needs to be passed around
        Editor() : m_EditorWindow(CreateRef<EditorWindow>()), m_EditorLayer(m_EditorWindow) {}
        ~Editor() = default;

        // because we are passing the window around, lets give it the flexibility of a shared pointer
        Ref<EditorWindow> m_EditorWindow;
        EditorLayer m_EditorLayer;

        // Actually starts our application.
        bool Run();

        void LoadConfig();
        void SaveConfig() const;

        YAML::Node m_ConfigFile;
    };
}

#endif //SLATE_EDITOR_H