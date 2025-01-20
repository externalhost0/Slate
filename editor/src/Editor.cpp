//
// Created by Hayden Rivas on 10/30/24.
//
#include <glad/glad.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include <Slate/Components.h>
#include <Slate/Entity.h>
#include <Slate/ShaderLibrary.h>

#include <nfd.h>

#include "Editor.h"
#include "Slate/Debug.h"

namespace Slate {
    // set of helper functions for error messages
    void glfwErrorCallback(int error_code, const char *description) {EXPECT(error_code, "%s", description)}
    void exitCallback() { printf("Editor Proper Exit Callback!\n"); }

    void setupErrorHandling() {
        glfwSetErrorCallback(glfwErrorCallback);
        atexit(exitCallback);
    }


    void Editor::Initialize() {
        setupErrorHandling(); // placed at beginning arbitarly

        // generate the main editor window, createScope builds it
        WindowSpecification winspec;
        winspec.m_VideoMode = VIDEO_MODE::WINDOWED;
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

		// file system dependency setup
		NFD_Init();


        // must be done before attaching, lets find a better way later
        // prefferably through constructors and not functions that are arbitraly called
        FramebufferSpecification fbSpec;
        // color, entity id, and depth
        fbSpec.Attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::DEPTH24STENCIL8 };
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
        const std::string dir = loadCurrentDirectoryLocation();
        renderMan.GetShaderManager().Load("Basic", "assets/shaders/vertex/static_PNT.vert", "assets/shaders/fragment/standard.frag");
        // for editor icons

        auto scene = m_ActiveContext->m_ActiveScene;


        Entity cube2 = scene->CreateEntity("Cube2");
        cube2.AddComponent<TransformComponent>(glm::vec3(0.0f, 5.0f, 2.0f));
        cube2.AddComponent<MeshComponent>(MeshCube("Basic"));

        Entity quad1 = scene->CreateEntity("Quad1");
        quad1.AddComponent<TransformComponent>(glm::vec3(0.0f, 5.0f, 7.0f));
        quad1.AddComponent<MeshComponent>(MeshQuad("Basic"));

        Entity plane1 = scene->CreateEntity("Plane1");
        plane1.AddComponent<TransformComponent>(glm::vec3{0.0f, 0.5f, 0.0f});
        plane1.GetComponent<TransformComponent>().Scale = {10.0f, 1.f, 10.0f};
        plane1.AddComponent<MeshComponent>(MeshPlane("Basic"));

        Entity object1 = scene->CreateEntity("Icosphere1");
        object1.AddComponent<TransformComponent>(glm::vec3(-6.0f, 3.0f, 0.0f));
        object1.AddComponent<MeshComponent>(MeshImport("Basic", ToDirectory("assets/models/icosphere.obj")));

        Entity object2 = scene->CreateEntity("Column1");
        object2.AddComponent<TransformComponent>(glm::vec3(-3.0f, 5.0f, -5.0f));
        object2.AddComponent<MeshComponent>(MeshImport("Basic", ToDirectory("assets/models/column.obj")));


		// test models on my system

//        Entity object3 = m_ActiveContext->m_ActiveScene->CreateEntity("Dragon1");
//        object3.AddComponent<TransformComponent>(glm::vec3(4.0f, -4.0f, -4.0f));
//        object3.GetComponent<TransformComponent>().Scale /= 30.0f;
//        object3.AddComponent<MeshComponent>(MeshImport("Basic", ToDirectory("assets/models/xyzrgb_dragon.obj")));
//        Entity meowrine = m_ActiveContext->m_ActiveScene->CreateEntity("Meowrine Figure");
//        meowrine.AddComponent<TransformComponent>();
//        meowrine.AddComponent<MeshComponent>(MeshImport("Basic", "../editor/assets/models/meowrine.obj"));



        Entity light1 = scene->CreateEntity("Light1");
        light1.AddComponent<TransformComponent>(glm::vec3(3.0f, 5.0f, -2.0f));
        light1.AddComponent<PointLightComponent>(glm::vec3{0.0f, 1.0f, 0.0f});

        Entity light2 = scene->CreateEntity("Light2");
        light2.AddComponent<TransformComponent>(glm::vec3(-3.0f, 2.0f, 4.0f));
        light2.AddComponent<PointLightComponent>(glm::vec3{0.0f, 0.0f, 1.0f});

        Entity spotlight1 = scene->CreateEntity("SpotLight1");
        spotlight1.AddComponent<TransformComponent>(glm::vec3{0.0f, 8.0f, 0.0f});
        spotlight1.AddComponent<SpotLightComponent>(glm::vec3{1.0f, 0.0f, 0.0f});

        Entity dirlight1 = scene->CreateEntity("Directionallight1");
        dirlight1.AddComponent<TransformComponent>(glm::vec3(-6.0f, 9.0f, -3.0f));
        dirlight1.GetComponent<TransformComponent>().Rotation = {45, 60, 0};
        dirlight1.AddComponent<DirectionalLightComponent>(glm::vec3{0.922, 0.918, 0.737});
        dirlight1.GetComponent<DirectionalLightComponent>().Intensity = 1.f;


        // link imgui callbacks to main editor window
        auto nativeWindow = SystemLocator::Get<WindowManager>().GetWindow().GetNativeWindow();
        ImGui_ImplGlfw_InstallCallbacks(nativeWindow);
    }

    void Editor::Loop() {
        auto nativeWindow = SystemLocator::Get<WindowManager>().GetWindow().GetNativeWindow();
        auto renderMan = SystemLocator::Get<RenderManager>();

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
            for (auto entity : m_ActiveContext->m_ActiveScene->GetAllEntitiesWith<TransformComponent, MeshComponent>()) {
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
                        model = glm::scale(model, glm::vec3(transform.Scale.x, transform.Scale.y, transform.Scale.z));
                    }
                    // this is dumb and needs to be replaced by ubos
                    // want to simplify (remove) this later
                    int id = static_cast<int>(entity.GetEntityHandle());
                    if (m_ActiveContext->m_ShaderMode == SHADERMODE::SOLIDWHITE) {
                        renderMan.GetShaderManager().Get("solid_color")->SetMat4("v_ModelMatrix", model);
                        renderMan.GetShaderManager().Get("solid_color")->SetInt("v_EntityID", static_cast<int>(id));
                        renderMan.GetShaderManager().Get("solid_color")->UseProgram();
                    }
                    else if (m_ActiveContext->m_ShaderMode == SHADERMODE::NORMALS_ONLY) {
                        renderMan.GetShaderManager().Get("normals_only")->SetMat4("v_ModelMatrix", model);
                        renderMan.GetShaderManager().Get("normals_only")->SetInt("v_EntityID", static_cast<int>(id));
                        renderMan.GetShaderManager().Get("normals_only")->UseProgram();
                    }
                    else {
                        renderMan.GetShaderManager().Get("Basic")->SetMat4("v_ModelMatrix", model);
                        renderMan.GetShaderManager().Get("Basic")->SetInt("v_EntityID", static_cast<int>(id));
                        renderMan.GetShaderManager().Get("Basic")->UseProgram();
                    }
                }
                // lighting!!

                // DIRECTIONAL LIGHT
                int i = 0;
                for (auto lightid : m_ActiveContext->m_ActiveScene->GetAllIDsWith<DirectionalLightComponent>()) {
                    Entity lightentity{lightid, m_ActiveContext->m_ActiveScene};
                    auto& light = lightentity.GetComponent<DirectionalLightComponent>();
                    auto& translight = lightentity.GetComponent<TransformComponent>();
                    auto lightingShader = renderMan.GetShaderManager().Get("Basic");

                    glm::vec3 forward = glm::vec3(0.0f, -1.0f, 0.0f);
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
                for (auto lightid : m_ActiveContext->m_ActiveScene->GetAllIDsWith<PointLightComponent>()) {
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

                // SPOT LIGHTS
                i = 0;
                for (auto lightid : m_ActiveContext->m_ActiveScene->GetAllIDsWith<SpotLightComponent>()) {
                    Entity lightentity{lightid, m_ActiveContext->m_ActiveScene};
                    auto& light = lightentity.GetComponent<SpotLightComponent>();
                    auto& translight = lightentity.GetComponent<TransformComponent>();

                    auto lightingShader = renderMan.GetShaderManager().Get("Basic");
                    std::string uniformName = "u_SpotLights[" + std::to_string(i) + "].";
                    lightingShader->SetVec3((uniformName + "Position"), &translight.Position);

                    lightingShader->SetVec3((uniformName + "Color"), &light.Color);
                    lightingShader->SetFloat((uniformName + "Intensity"), light.Intensity);

                    glm::vec3 forward = glm::vec3(0.0f, -1.0f, 0.0f);
                    // Compute the rotation quaternion from Euler angles
                    glm::quat rotationQuat = glm::quat(glm::radians(translight.Rotation));
                    // Rotate the forward vector
                    glm::vec3 direction = rotationQuat * forward;

                    lightingShader->SetVec3((uniformName + "Direction"), &direction);
                    lightingShader->SetFloat((uniformName + "Blend"), light.Blend);
                    lightingShader->SetFloat((uniformName + "Size"), light.Size);
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
            // ALWAYS DRAW BEFORE gui, viewport depends on it
            m_GUI->PostDrawUpdate();




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
        NFD_Quit();

        m_GUI->OnDetach();
        // imgui shutdown
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        // glfw shutdown
        SystemLocator::Get<WindowManager>().GetWindow().DestroyWindow();
        glfwTerminate();
    }

}