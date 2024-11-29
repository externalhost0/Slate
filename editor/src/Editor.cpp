//
// Created by Hayden Rivas on 10/30/24.
//
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <Slate/Components.h>
#include <Slate/Entity.h>
#include <Slate/Time.h>
#include <Slate/ShaderManager.h>

#include <fstream>

#include "Editor.h"

namespace Slate {
    // set of helper functions for error messages
    void glfwErrorCallback(int error_code, const char *description) {EXPECT(error_code, "%s", description)}
    void exitCallback() { printf("Editor Exit Callback!\n"); }
    void setupErrorHandling() {
        glfwSetErrorCallback(glfwErrorCallback);
        atexit(exitCallback);
    }


    void Editor::Init() {
        setupErrorHandling();

        m_EditorLayer = CreateScope<EditorLayer>();
        // generate the main editor window, createScope builds it
        WindowSpecification winspec;
        winspec.m_VideoMode = WINDOWED;
        winspec.m_IsResizeable = true;
        winspec.m_WindowTitle = "Slate Editor";
        m_MainWindow = CreateScope<Window>(winspec);

        // idk where to put this
        Renderer::Init();


        // must be done before attaching, lets find a better way later
        // prefferably through constructors and not functions that are arbitraly called
        m_EditorLayer->Bootstrap();

        // i believe the order should go like this, be open to moving entity creation around
        // tests scene on startup, make a cube and give it the following components

        Renderer::GetShaderManager().Setup();
        Renderer::GetShaderManager().Load("Basic", "../editor/assets/shaders/static.vert", "../editor/assets/shaders/basic.frag");

         // all of the above does nothing


        Entity cube1 = m_EditorLayer->m_ActiveContext->m_ActiveScene->CreateEntity("Cube1");
        cube1.AddComponent<TransformComponent>(glm::vec3(2.0f, 0.0f, 0.0f));
        cube1.AddComponent<MeshComponent>(MeshCube("Basic"));

        Entity cube2 = m_EditorLayer->m_ActiveContext->m_ActiveScene->CreateEntity("Cube2");
        cube2.AddComponent<TransformComponent>(glm::vec3(0.0f, 5.0f, 2.0f));
        cube2.AddComponent<MeshComponent>(MeshCube("Basic"));

        Entity plane1 = m_EditorLayer->m_ActiveContext->m_ActiveScene->CreateEntity("Plane1");
        plane1.AddComponent<TransformComponent>(glm::vec3(0.0f, -5.0f, 0.0f));
        plane1.GetComponent<TransformComponent>().Rotation = {-90.0f, 0.0f, 0.0f};
        plane1.GetComponent<TransformComponent>().Scale = {10.0f, 10.f, 0.0f};
        plane1.AddComponent<MeshComponent>(MeshPlane("Basic"));

        Entity object1 = m_EditorLayer->m_ActiveContext->m_ActiveScene->CreateEntity("Object1");
        object1.AddComponent<TransformComponent>(glm::vec3(-6.0f, 0.0f, 0.0f));
        object1.AddComponent<MeshComponent>(MeshImport("Basic", "../editor/assets/models/icosphere.obj"));

        Entity object2 = m_EditorLayer->m_ActiveContext->m_ActiveScene->CreateEntity("Column1");
        object2.AddComponent<TransformComponent>(glm::vec3(-3.0f, 0.0f, -5.0f));
        object2.AddComponent<MeshComponent>(MeshImport("Basic", "../editor/assets/models/column.obj"));

        // link imgui callbacks to main editor window
        ImGui_ImplGlfw_InstallCallbacks(m_MainWindow->GetNativeWindow());
    }

    void Editor::Loop() {
        while (!glfwWindowShouldClose(m_MainWindow->GetNativeWindow())) {
            Time::CalculateDeltaTime();
            glfwPollEvents();
            // actual stuff

            m_EditorLayer->OnUpdate();

            // basic glfw things
            glfwSwapBuffers(m_MainWindow->GetNativeWindow());
        }
    }

    void Editor::Shutdown() {
//        SaveConfig();
        // editor personal shutdown
        m_EditorLayer->OnDetach();
        // imgui shutdown
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        // glfw shutdown
        glfwDestroyWindow(m_MainWindow->GetNativeWindow());
        glfwTerminate();
    }

    // most important function ig, this is the cycle order of the program
    void Editor::Run() {
        Init();
        Loop();
        Shutdown();
    }

    // some helper functions, move later
    void Editor::SaveConfig() const {
        // can we access it
        std::ofstream fout("../editor/config.yaml");
        if (!fout.is_open()) {
            std::cerr << "Unable to open config file for writing!" << std::endl;
            return;
        }
        // write and close
        fout << m_ConfigFile;
        fout.close();
    }
    void Editor::LoadConfig() {
//        m_ConfigFile = YAML::LoadFile("../editor/config.yaml");
//        std::cout << m_ConfigFile << std::endl; // test print
    }


}