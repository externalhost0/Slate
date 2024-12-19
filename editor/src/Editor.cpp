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
#include <Slate/ShaderLibrary.h>

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

    int NUM_POINT_LIGHTS = 2;

    void Editor::Initialize() {
        setupErrorHandling(); // placed at beginning arbitarly


        // generate the main editor window, createScope builds it
        WindowSpecification winspec;
        winspec.m_VideoMode = WINDOWED;
        winspec.m_IsResizeable = true;
        winspec.m_WindowTitle = "Slate Editor";
        Window myFirstWindow(winspec);
        myFirstWindow.ConstructWindow();
        SystemLocator::Get<WindowManager>().SetMainWindow(myFirstWindow);

        // needs to be done after window creation, specifically glad init
        glEnable(GL_DEPTH_TEST); // enable depth testing
        glEnable(GL_STENCIL_TEST); // enable stencil testing (for outlines)
        glEnable(GL_CULL_FACE); // set culling so only front faces show
        glCullFace(GL_BACK); // cull back
        glFrontFace(GL_CCW); // tell opengl all of our stuff is counter clockwise

        // idk where to put this
        // or if it should be put anywhere at all

        // must be done before attaching, lets find a better way later
        // prefferably through constructors and not functions that are arbitraly called
        FramebufferSpecification fbSpec;
        // color, entity id, and depth
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        fbSpec.Samples = 1;
        m_Framebuffer = CreateRef<Framebuffer>(fbSpec);

        m_ActiveContext = CreateRef<Context>();
        m_ActiveContext->m_ActiveScene = new Scene();
        m_ActiveContext->m_ActiveEntity = Entity::Null;

        m_GUI = CreateRef<EditorGUI>();
        m_GUI->OnAttach(m_ActiveContext, m_Framebuffer);


        // literally just loads some shaders we want the editor to always have access to
        auto& renderMan = SystemLocator::Get<RenderManager>();
        renderMan.GetShaderManager().Setup();
        // the shader all of these examlpes will be using
        renderMan.GetShaderManager().Load("Basic", "../editor/assets/shaders/vertex/static.vert", "../editor/assets/shaders/fragment/basic.frag");
        // for editor icons

        auto& fontMan = SystemLocator::Get<FontManager>();
        fontMan.AddFont("NotoSans-Regular", "../editor/assets/fonts/lucide.ttf");
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // use a single byte
//
//        for (unsigned int c = ICON_MIN_LC; c <= ICON_MAX_16_LC; c++) {
//            fontMan.GetFont("NotoSans-Regular")->BuildGlyph(c);
//        }
//        FT_Done_Face(fontMan.GetFont("NotoSans-Regular")->GetFace());
//


        Entity cube1 = m_ActiveContext->m_ActiveScene->CreateEntity("Cube1");
        cube1.AddComponent<TransformComponent>(glm::vec3(2.0f, 0.0f, 0.0f));
        cube1.AddComponent<MeshComponent>(MeshCube("Basic"));

        Entity cube2 = m_ActiveContext->m_ActiveScene->CreateEntity("Cube2");
        cube2.AddComponent<TransformComponent>(glm::vec3(0.0f, 5.0f, 2.0f));
        cube2.AddComponent<MeshComponent>(MeshCube("Basic"));

        Entity plane1 = m_ActiveContext->m_ActiveScene->CreateEntity("Plane1");
        plane1.AddComponent<TransformComponent>(glm::vec3(0.0f, -5.0f, 0.0f));
        plane1.GetComponent<TransformComponent>().Rotation = {-90.0f, 0.0f, 0.0f};
        plane1.GetComponent<TransformComponent>().Scale = {10.0f, 10.f, 0.0f};
        plane1.AddComponent<MeshComponent>(MeshPlane("Basic"));

        Entity object1 = m_ActiveContext->m_ActiveScene->CreateEntity("Icosphere1");
        object1.AddComponent<TransformComponent>(glm::vec3(-6.0f, 0.0f, 0.0f));
        object1.AddComponent<MeshComponent>(MeshImport("Basic", "../editor/assets/models/icosphere.obj"));

        Entity object2 = m_ActiveContext->m_ActiveScene->CreateEntity("Column1");
        object2.AddComponent<TransformComponent>(glm::vec3(-3.0f, 0.0f, -5.0f));
        object2.AddComponent<MeshComponent>(MeshImport("Basic", "../editor/assets/models/column.obj"));

//        Entity object3 = m_ActiveContext->m_ActiveScene->CreateEntity("Dragon1");
//        object3.AddComponent<TransformComponent>(glm::vec3(4.0f, -4.0f, -4.0f));
//        object3.GetComponent<TransformComponent>().Scale /= 30.0f;
//        object3.AddComponent<MeshComponent>(MeshImport("Basic", "../editor/assets/models/xyzrgb_dragon.obj"));

//        Entity meowrine = m_ActiveContext->m_ActiveScene->CreateEntity("Meowrine Figure");
//        meowrine.AddComponent<TransformComponent>();
//        meowrine.AddComponent<MeshComponent>(MeshImport("Basic", "../editor/assets/models/meowrine.obj"));



        Entity light1 = m_ActiveContext->m_ActiveScene->CreateEntity("Light1");
        light1.AddComponent<TransformComponent>(glm::vec3(3.0f, 5.0f, -2.0f));
        light1.AddComponent<PointLightComponent>(glm::vec3{0.0f, 1.0f, 0.0f});

        Entity light2 = m_ActiveContext->m_ActiveScene->CreateEntity("Light2");
        light2.AddComponent<TransformComponent>(glm::vec3(-3.0f, 2.0f, 4.0f));
        light2.AddComponent<PointLightComponent>(glm::vec3{0.0f, 0.0f, 1.0f});

        Entity dirlight1 = m_ActiveContext->m_ActiveScene->CreateEntity("Directionallight1");
        dirlight1.AddComponent<TransformComponent>(glm::vec3(-6.0f, 9.0f, -3.0f));
        dirlight1.GetComponent<TransformComponent>().Rotation = {150, 80, 0};
        dirlight1.AddComponent<DirectionalLightComponent>(glm::vec3{0.922, 0.918, 0.737});
        dirlight1.GetComponent<DirectionalLightComponent>().Intensity = 1.f;


        // link imgui callbacks to main editor window
        auto nativeWindow = SystemLocator::Get<WindowManager>().GetWindow().GetNativeWindow();
        ImGui_ImplGlfw_InstallCallbacks(nativeWindow);
    }

    void Editor::Loop() {
        auto nativeWindow = SystemLocator::Get<WindowManager>().GetWindow().GetNativeWindow();
        auto renderMan = SystemLocator::Get<RenderManager>();
        auto& fontMan = SystemLocator::Get<FontManager>();

        glfwPollEvents();
        // actual stuff //
        {
            // resizing logic needs to be handled first

            // PREP
            m_Framebuffer->Bind();
            renderMan.ClearColor();
            renderMan.ClearStencil();
            renderMan.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            m_Framebuffer->ClearAttachment(1, -1);

            // ACTION
            // - - - - - -


            // THIS RENDER SEQUENCE SHOULD ONLY, AND I SAY ONLY FIRMLY, BE FOR EXPLICIT USER CREATED OBJECTS/ENTITIES
            // that means no editor only meshs should be rendered here
            // draw meshes, only if they have transform lol
            for (auto id : m_ActiveContext->m_ActiveScene->GetAllEntitiesWith<TransformComponent, MeshComponent>()) {
                Entity entity{id, m_ActiveContext->m_ActiveScene};
                // shader update (requires transform, for now)
                {
                    auto model = glm::mat4(1.0f);
                    // transform processing
                    {
                        auto &transform = entity.GetComponent<TransformComponent>();
                        model = glm::translate(model, glm::vec3(transform.Position.x, transform.Position.y, transform.Position.z));

                        // for proper rotation rendering use quaternions
                        glm::quat rotationQuat = glm::quat(glm::radians(glm::vec3(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z)));
                        model = model * glm::mat4_cast(rotationQuat);

                        // if mesh is 2D, make scaling on the Z do nothing
                        if (entity.GetComponent<MeshComponent>().m_IsYScalable)
                            model = glm::scale(model, glm::vec3(transform.Scale.x, 1.0f, transform.Scale.z));
                        else
                            model = glm::scale(model, glm::vec3(transform.Scale.x, transform.Scale.y, transform.Scale.z));
                    }
                    // this is dumb and needs to be replaced by ubos
                    // want to simplify (remove) this later
                    if (m_ActiveContext->m_ShaderMode == SHADERMODE::SOLIDWHITE) {
                        renderMan.GetShaderManager().Get("solid_color")->SetMat4("v_ModelMatrix", model);
                        renderMan.GetShaderManager().Get("solid_color")->SetInt("v_EntityID", (int) id);
                        renderMan.GetShaderManager().Get("solid_color")->UseProgram();
                    }
                    else if (m_ActiveContext->m_ShaderMode == SHADERMODE::NORMALS_ONLY) {
                        renderMan.GetShaderManager().Get("normals_only")->SetMat4("v_ModelMatrix", model);
                        renderMan.GetShaderManager().Get("normals_only")->SetInt("v_EntityID", (int) id);
                        renderMan.GetShaderManager().Get("normals_only")->UseProgram();
                    }
                    else {
                        renderMan.GetShaderManager().Get("Basic")->SetMat4("v_ModelMatrix", model);
                        renderMan.GetShaderManager().Get("Basic")->SetInt("v_EntityID", (int) id);
                        renderMan.GetShaderManager().Get("Basic")->UseProgram();
                    }
                }
                // lighting!!

                // DIRECTIONAL LIGHT
                int i = 0;
                for (auto lightid : m_ActiveContext->m_ActiveScene->GetAllEntitiesWith<DirectionalLightComponent>()) {
                    Entity lightentity{lightid, m_ActiveContext->m_ActiveScene};
                    auto& light = lightentity.GetComponent<DirectionalLightComponent>();
                    auto& translight = lightentity.GetComponent<TransformComponent>();
                    auto lightingShader = renderMan.GetShaderManager().Get("Basic");

                    glm::vec3 forward = glm::vec3(0.0f, 1.0f, 0.0f);
                    // Compute the rotation quaternion from Euler angles
                    glm::quat rotationQuat = glm::quat(glm::radians(translight.Rotation));
                    // Rotate the forward vector
                    glm::vec3 direction = rotationQuat * forward;


                    std::string uniformName = "u_DirectionalLight.";
                    lightingShader->SetVec3((uniformName + "Color"), &light.Color);
                    lightingShader->SetFloat((uniformName + "Intensity"), light.Intensity);
                    lightingShader->SetVec3((uniformName + "Direction"), &direction);
                    i++;
                }

                // POINT LIGHTS
                i = 0;
                for (auto lightid : m_ActiveContext->m_ActiveScene->GetAllEntitiesWith<PointLightComponent>()) {
                    Entity lightentity{lightid, m_ActiveContext->m_ActiveScene};
                    auto& light = lightentity.GetComponent<PointLightComponent>();
                    auto& translight = lightentity.GetComponent<TransformComponent>();

                    auto lightingShader = renderMan.GetShaderManager().Get("Basic");
                    std::string uniformName = "u_PointLights[" + std::to_string(i) + "].";
                    lightingShader->SetVec3((uniformName + "Position"), &translight.Position);
                    lightingShader->SetVec3((uniformName + "Color"), &light.Color);
                    lightingShader->SetFloat((uniformName + "Intensity"), light.Intensity);
                    lightingShader->SetFloat((uniformName + "Range"), light.Range);
                    i++;
                }

                // for the mesh instances the component has
                for (const auto &el: entity.GetComponent<MeshComponent>().m_Meshes) {
                    el.Bind();
                    el.DrawMesh();
                    el.Unbind();
                }
            }
            // - - - - -
            // ALWAYS DRAW BEFORE Gui, viewport depends on it
            m_GUI->PostDrawUpdate();

            // opengl 4.1 doesnt have a callback :(
            static bool called = false;
            GLenum err = glGetError();
            if (err != GL_NO_ERROR && !called) {
                fprintf(stderr, "OpenGL Error: %u\n", err);
                called = true;
            }


            // CLEAN
            m_Framebuffer->Unbind();
            m_GUI->DrawFinish(); // must be at end, as it wraps all render passes with the gui lastly
        }
        //          //
        glfwSwapBuffers(nativeWindow);
        if (glfwWindowShouldClose(nativeWindow)) {
            shouldStopLoop = true;
        }
    }

    void Editor::Shutdown() {
        // editor personal shutdown
        m_GUI->OnDetach();
        // imgui shutdown
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        // glfw shutdown
        SystemLocator::Get<WindowManager>().GetWindow().DestroyWindow();
        glfwTerminate();
    }

    // some helper functions, move later
    void Editor::SaveConfig() const {
        // can we access it
        std::ofstream fout("../editor/config.yaml");
        if (!fout.is_open()) {
            fprintf(stderr, "Unable to open config file for writing!\n");
            return;
        }
        // write and close
//        fout << m_ConfigFile;
        fout.close();
    }
    void Editor::LoadConfig() {
//        m_ConfigFile = YAML::LoadFile("../editor/config.yaml");
//        std::cout << m_ConfigFile << std::endl; // test print
    }


}