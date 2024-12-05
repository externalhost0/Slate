//
// Created by Hayden Rivas on 10/30/24.
//
#include <glad/glad.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../external/IconFontCppHeaders/IconsLucide.h"

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
    void APIENTRY OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        std::cerr << "OpenGL Debug Message:" << std::endl;
        std::cerr << "Source: " << source << ", Type: " << type << ", ID: " << id << std::endl;
        std::cerr << "Severity: " << severity << std::endl;
        std::cerr << "Message: " << message << std::endl;
    }
    void setupErrorHandling() {
        glfwSetErrorCallback(glfwErrorCallback);
        atexit(exitCallback);
    }


    void Editor::Init() {
        setupErrorHandling(); // placed at beginning arbitarly


        m_EditorLayer = CreateScope<EditorLayer>();
        // generate the main editor window, createScope builds it
        WindowSpecification winspec;
        winspec.m_VideoMode = WINDOWED;
        winspec.m_IsResizeable = true;
        winspec.m_WindowTitle = "Slate Editor";
        m_MainWindow = CreateScope<Window>(winspec);

        // idk where to put this
        // or if it should be put anywhere at all
        Renderer::Init();


        // must be done before attaching, lets find a better way later
        // prefferably through constructors and not functions that are arbitraly called
        m_EditorLayer->Bootstrap();

        // literally just loads some shaders we want the editor to always have access to
        Renderer::GetShaderManager().Setup();
        // the shader all of these examlpes will be using
        Renderer::GetShaderManager().Load("Basic", "../editor/assets/shaders/static.vert", "../editor/assets/shaders/basic.frag");
        // for editor icons
        Renderer::GetShaderManager().Load("Icon", "../editor/assets/shaders/static_editor.vert", "../editor/assets/shaders/icon.frag");


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

        Entity light1 = m_EditorLayer->m_ActiveContext->m_ActiveScene->CreateEntity("Light1");
        light1.AddComponent<TransformComponent>(glm::vec3(3.0f, 5.0f, -2.0f));
//        light1.AddComponent<TextComponent>(ICON_LC_LIGHTBULB);
        light1.AddComponent<LightComponent>(PointLight());

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
    // however has no reason for the user to interfere, as this is universal
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