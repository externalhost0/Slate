//
// Created by Hayden Rivas on 10/30/24.
//
#ifndef SLATE_EDITOR_H
#define SLATE_EDITOR_H
// things the editor requires
// 1. a window
// 2. a gui
// any other essentials will be relocated
#include <Slate/Scope.h>
#include <yaml-cpp/yaml.h>

#include "EditorLayer.h"

namespace Slate {
    class Editor {
    private:
        // processing of engine steps
        void Init();
        void Loop();
        void Shutdown();
    public:
        // actually start the app
        void Run();
    private:
        static inline Scope<Window> m_MainWindow; // delay construction
        Scope<EditorLayer> m_EditorLayer;
    public:
        static Window& GetEditorWindow() { return *m_MainWindow; }; // required
        std::vector<Scope<Window>> m_SubWindows; // optional runtime construction


        // go somewhere later
        void LoadConfig();
        void SaveConfig() const;
        YAML::Node m_ConfigFile;
    };
}
#endif //SLATE_EDITOR_H