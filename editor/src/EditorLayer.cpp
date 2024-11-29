//
// Created by Hayden Rivas on 11/5/24.
//
#include <Slate/Components.h>
#include "EditorLayer.h"

namespace Slate {
    void EditorLayer::Bootstrap() {

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
    }

    void EditorLayer::OnDetach() const {
        // on detach for each panel
        m_GUI->OnDetach();
    }

    void EditorLayer::OnImGuiUpdate() {
    }


    void EditorLayer::OnUpdate() {
        // Loop Series
        {
            // resizing logic needs to be handled first

            // PREP
            m_Framebuffer->Bind();
            Renderer::ClearColor();
            Renderer::ClearStencil();
            Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            m_Framebuffer->ClearAttachment(1, -1);


            // ACTION
            // - - - - - -
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
                        if (entity.GetComponent<MeshComponent>().m_IsZScalable)
                            model = glm::scale(model, glm::vec3(transform.Scale.x, transform.Scale.y, 0.0f));
                        else
                            model = glm::scale(model, glm::vec3(transform.Scale.x, transform.Scale.y, transform.Scale.z));
                    }
                    Renderer::GetShaderManager().Get(entity.GetComponent<MeshComponent>().m_ShaderName)->setMat4("v_ModelMatrix", model);
                    Renderer::GetShaderManager().Get(entity.GetComponent<MeshComponent>().m_ShaderName)->setInt("v_EntityID", (int)(entt::entity)entity);

                    Renderer::GetShaderManager().Get("solid_color")->setMat4("v_ModelMatrix", model);
                    Renderer::GetShaderManager().Get("solid_color")->setInt("v_EntityID", (int)(entt::entity)entity);
                }
                if (m_ActiveContext->m_ShaderMode == SHADERMODE::SOLIDWHITE)
                    Renderer::GetShaderManager().Get("solid_color")->UseProgram();
                else
                    Renderer::GetShaderManager().Get(entity.GetComponent<MeshComponent>().m_ShaderName)->UseProgram();

                for (const auto &el: entity.GetComponent<MeshComponent>().m_Meshes) {
                    el.DrawMesh();
                }
            }
            // - - - - -
            // ALWAYS DRAW BEFORE Gui, viewport depends on it
            m_GUI->PostDrawUpdate();
            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                std::cerr << "OpenGL Error: " << err << std::endl;
            }


            // CLEAN
            m_Framebuffer->Unbind();
            m_GUI->DrawFinish(); // must be at end, as it wraps all render passes with the gui lastly

        }
    }
}
