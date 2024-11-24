//
// Created by Hayden Rivas on 11/5/24.
//
#include <GL/glew.h>
#include "EditorLayer.h"
#include "yaml-cpp/yaml.h"

namespace Slate {
    void EditorLayer::Bootstrap() {
        FramebufferSpecification fbSpec;
        // color, entity id, and depth
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = CreateRef<Framebuffer>(fbSpec);

        m_ActiveContext = CreateRef<Context>();
        m_ActiveContext->m_ActiveScene = new Scene();
        m_ActiveContext->m_ActiveEntity = Entity::Null;

        m_Renderer = CreateRef<Renderer>();
        m_Renderer->Init();

        m_GUI = new EditorGUI(m_EditorWindow);
        m_GUI->OnAttach(m_ActiveContext, m_Framebuffer);
    }

    void EditorLayer::OnDetach() const {
        // on detach for each panel
        m_GUI->OnDetach();
    }

    void EditorLayer::OnUpdate() {
        // Loop Series
        {
            // PREP
            m_Framebuffer->Bind();
            Renderer::ClearColor();
            Renderer::ClearStencil();
            Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            m_Framebuffer->ClearAttachment(1, -1);

            // ACTION
            // which is update uniforms and draw every frame
            // - - - - - -
            // draw primitives
            for (auto id : m_ActiveContext->m_ActiveScene->GetAllEntitiesWith<TransformComponent, PrimitiveComponent>()) {
                Entity entity{id, m_ActiveContext->m_ActiveScene};
                // shader update (requires transform, for now)
                {
                    auto model = glm::mat4(1.0f);
                    // transform processing
                    {
                        auto &transform = entity.GetComponent<TransformComponent>();
                        // just translate according to its current position in the component
                        model = glm::translate(model, glm::vec3(transform.Position.x, transform.Position.y, transform.Position.z));

                        // for proper rotation rendering use quaternions
                        glm::quat rotationQuat = glm::quat(glm::radians(glm::vec3(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z)));
                        model = model * glm::mat4_cast(rotationQuat);

                        // if mesh is said to be 2D, make scaling on the Z do nothing
                        if (entity.GetComponent<PrimitiveComponent>().m_IsZScalable)
                            model = glm::scale(model, glm::vec3(transform.Scale.x, transform.Scale.y, 0.0f));
                        else
                            model = glm::scale(model, glm::vec3(transform.Scale.x, transform.Scale.y, transform.Scale.z));
                    }
                    entity.GetComponent<PrimitiveComponent>().m_Shader.setMat4("v_ModelMatrix", model);
                    entity.GetComponent<PrimitiveComponent>().m_Shader.setInt("v_EntityID", (int) (entt::entity) entity);
                }
                entity.GetComponent<PrimitiveComponent>().m_Shader.UseProgram();
                entity.GetComponent<PrimitiveComponent>().m_Mesh.DrawMesh();
            }
            // draw models
            for (auto id : m_ActiveContext->m_ActiveScene->GetAllEntitiesWith<TransformComponent, ModelComponent>()) {
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
                        if (entity.GetComponent<ModelComponent>().m_IsZScalable) {
                            model = glm::scale(model, glm::vec3(transform.Scale.x, transform.Scale.y, 0.0f));
                        } else {
                            model = glm::scale(model, glm::vec3(transform.Scale.x, transform.Scale.y, transform.Scale.z));
                        }
                    }
                    entity.GetComponent<ModelComponent>().m_Shader.setMat4("v_ModelMatrix", model);
                    entity.GetComponent<ModelComponent>().m_Shader.setInt("v_EntityID", (int) (entt::entity) entity);
                }
                entity.GetComponent<ModelComponent>().m_Shader.UseProgram();
                for (const auto& el : entity.GetComponent<ModelComponent>().m_Meshes) {
                    el.DrawMesh();
                }
            }
            // - - - - -
            m_GUI->OnUpdate(); // ALWAYS DRAW BEFORE GUI, viewport depends on it

            // CLEAN
            m_Framebuffer->Unbind();
            m_GUI->DrawFinish(); // must be at end, as it wraps all render passes with the gui lastly

        }
    }
}
