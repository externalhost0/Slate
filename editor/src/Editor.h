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
    public:
        // dont have much of a reason for member variables to be exposed in a class like this,
        // everything should be fairly abstracted
        YAML::Node m_ConfigFile;
    public:
        // go somewhere later
        void LoadConfig();
        void SaveConfig() const;
        // actually start the app
        void Run();
        static Window& GetEditorWindow() {
            assert(m_MainWindow && "Main window is not initialized!");
            return *m_MainWindow;
        };
        std::vector<Scope<Window>> m_SubWindows; // optional runtime construction

    private:
        // processing of engine steps
        void Init();
        void Loop();
        void Shutdown();
    private:
        static inline Scope<Window> m_MainWindow; // delay construction
        Scope<EditorLayer> m_EditorLayer;
    };
}
#endif //SLATE_EDITOR_H