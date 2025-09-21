//
// Created by Hayden Rivas on 11/10/24.
//
#include <glad/glad.h>
#include "../../../external/IconFontCppHeaders/IconsLucide.h"

#include <Slate/Components.h>
#include <Slate/Entity.h>

#include "PropertiesPanel.h"
#include "../Fonts.h"
#include "../ImGuiSnippets.h"

#include <nfd.h>
#include <iostream>

namespace Slate {
    std::string getUniformTypeName(GLenum type) {
        switch (type) {
            case GL_FLOAT: return "GL_FLOAT";
            case GL_FLOAT_VEC2: return "GL_FLOAT_VEC2";
            case GL_FLOAT_VEC3: return "GL_FLOAT_VEC3";
            case GL_FLOAT_VEC4: return "GL_FLOAT_VEC4";
            case GL_INT: return "GL_INT";
            case GL_INT_VEC2: return "GL_INT_VEC2";
            case GL_INT_VEC3: return "GL_INT_VEC3";
            case GL_INT_VEC4: return "GL_INT_VEC4";
            case GL_BOOL: return "GL_BOOL";
            case GL_BOOL_VEC2: return "GL_BOOL_VEC2";
            case GL_BOOL_VEC3: return "GL_BOOL_VEC3";
            case GL_BOOL_VEC4: return "GL_BOOL_VEC4";
            case GL_FLOAT_MAT2: return "GL_FLOAT_MAT2";
            case GL_FLOAT_MAT3: return "GL_FLOAT_MAT3";
            case GL_FLOAT_MAT4: return "GL_FLOAT_MAT4";
            case GL_SAMPLER_2D: return "GL_SAMPLER_2D";
            case GL_SAMPLER_CUBE: return "GL_SAMPLER_CUBE";
            default: return "Unknown Type";
        }
    }

    void ComponentCore(Entity* entity) {
        // copy string to buffer func
        char buffer[128] = "";
        std::string currentName = entity->GetComponent<CoreComponent>().name;
        strncpy(buffer, currentName.c_str(), sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination


        ImGui::PushFont(Fonts::iconLargeFont);
        ImGui::Text(ICON_LC_BOX);
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.0f);
        ImGui::PushFont(Fonts::largeboldFont);
        if (ImGui::InputText("##RenameInput", buffer, IM_ARRAYSIZE(buffer),ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
            entity->GetComponent<CoreComponent>().name = buffer;
        }
        ImGui::PopFont();

    }

    void ComponentTransformation(Entity* entity) {
        auto &transform = entity->GetComponent<TransformComponent>();
        float width = 100.0f;

        Vector3Drag("Position", transform.Position, width, 0.0f, 0.1f);

        auto model = glm::mat4(1.0f);
        glm::quat rotationQuat = glm::quat(glm::radians(glm::vec3(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z)));
        glm::mat4 localRotationMatrix = glm::mat4_cast(rotationQuat);
        glm::vec3 localRotation = glm::eulerAngles(rotationQuat);
        localRotation = glm::degrees(localRotation);
        // broken as of now
        Vector3Drag("Rotation", localRotation, width, 0.0f, 0.1f);
        // now set it back to the rotation
        transform.Rotation = localRotation;

        if (!transform.NoScale)
            Vector3Drag("Scale", transform.Scale, width, 1.0f, 0.1f);
    }

    // uniforms directly set by the editor ONLY
    const char* unedit_uniforms[] = { "v_EntityID", "v_ProjectionMatrix", "v_ViewMatrix", "v_ModelMatrix", "v_ViewPos" };
    void ComponentMesh(Entity* entity) {
        ImGui::PushFont(Fonts::boldFont);
        ImGui::Text("Mesh Specifications: ");
        ImGui::PopFont();
        ImGui::Separator();

        auto &mesh = entity->GetComponent<MeshComponent>();
        if (entity->HasComponent<MeshComponent>()) {
            ImGui::Text("%s", mesh.m_ShapeName.c_str());
            ImGui::Text("%s", entity->GetComponent<MeshComponent>().m_Directory.c_str());
        } else {
            ImGui::Text("%s", mesh.m_ShapeName.c_str());
        }

        ImGui::Spacing();


        const char* items[] = { "Cube", "Quad", "Plane", "User" };
        static int selectedItem = -1; // -1 means no item is selected
        if (ImGui::BeginCombo("Select an option", selectedItem == -1 ? "None" : items[selectedItem])) {
            for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
                bool isSelected = (selectedItem == i);
                if (ImGui::Selectable(items[i], isSelected)) {
                    selectedItem = i;

                    // Perform an action based on the selected item
                    switch (selectedItem) {
                        case 0:
                            std::cout << "Option 1 selected: Performing action 1\n";
                            break;
                        case 1:
                            std::cout << "Option 2 selected: Performing action 2\n";
                            break;
                        case 2:
                            std::cout << "Option 3 selected: Performing action 3\n";
                            break;
                        case 3:
                            std::cout << "Option 4 selected: Performing action 4\n";
                            break;
                        default:
                            break;
                    }
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Import Mesh")) {
            nfdu8char_t *outPath;
            nfdu8filteritem_t filters[4] = {
                    { "OBJ", "obj" },
                    { "STL", "stl" },
                    { "GTLF", "gtlf" },
                    { "FBX", "fbx" }
            };
            nfdopendialogu8args_t args = {nullptr};
            args.filterList = filters;
            args.filterCount = 4;
            nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
            if (result == NFD_OKAY) {
                printf("Successfully Imported: '%s'\n", outPath);
                // remakes mesh component
                entity->RemoveComponent<MeshComponent>();
                entity->AddComponent<MeshComponent>(MeshImport("Basic", outPath));

                NFD_FreePathU8(outPath);
            }
            else {
                fprintf(stderr, "Error: %s\n", NFD_GetError());
            }

        }

//        {
//            ImGui::PushFont(Fonts::boldFont);
//            ImGui::Text("Shader Specifications: ");
//            ImGui::PopFont();
//            ImGui::Separator();
//            ImGui::PushFont(Fonts::italicFont);
//            ImGui::Text("Shader Files: ");
//            ImGui::PopFont();
//            float indent = 50.0f;
//            ImGui::Indent(indent);
//            auto shader = *SystemLocator::Get<RenderManager>().GetShaderManager().Get(
//                    entity->GetComponent<MeshComponent>().m_ShaderName);
//            ImGui::Text("%s", shader.m_VertexFile.c_str());
//            ImGui::Text("%s", shader.m_FragmentFile.c_str());
//            ImGui::Unindent(indent);
//
//            // - - - - - - - - //
//
//            // for every uniform display it correctly
//            GLint count;
//            unsigned int progID = shader.GetProgramIDCopy();
//            glGetProgramiv(progID, GL_ACTIVE_UNIFORMS, &count);
//
//            ImGui::Text("Available Uniforms:");
//            for (GLint i = 0; i < count; i++) {
//                char name[128];
//                GLint size; // m_Count of the variable
//                GLenum type; // type of the variable (float, vec3 or mat4, etc)
//                GLsizei length; // name length
//                glGetActiveUniform(progID, (GLuint) i, sizeof(name), &length, &size, &type, name);
//                // if uniform is one that is set internally by the editor mark it as so with some text
//                bool isUneditable = false;
//                for (const char *unedit_name: unedit_uniforms) {
//                    if (std::strcmp(name, unedit_name) == 0) {
//                        isUneditable = true;
//                        break;
//                    }
//                }
//                if (isUneditable) continue;
//
//                ImGui::PushID(typeid(MeshComponent).hash_code() + (int) (uint64_t) entity->GetUUID());
//                ImGui::Text("%s : %s", getUniformTypeName(type).c_str(), name);
//                ImGui::SameLine();
//
//                float fspeed = 0.1f;
//                // most common types of user defined uniforms
//                if (type == GL_FLOAT_VEC4) {
//                    float val[4];
//                    glm::vec4 uniform = shader.getVec4(name);
//                    val[0] = uniform.r;
//                    val[1] = uniform.g;
//                    val[2] = uniform.b;
//                    val[3] = uniform.a;
//                    if (ImGui::ColorButton(name, ImVec4(val[0], val[1], val[2], val[3])))
//                        ImGui::OpenPopup("hi-picker");
//                    if (ImGui::BeginPopup("hi-picker",
//                                          ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
//                        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) ImGui::CloseCurrentPopup();
//                        ImGui::ColorPicker4("##picker", val);
//                        shader.SetVec4(name, val[0], val[1], val[2], val[3]);
//                        ImGui::EndPopup();
//                    }
//                } else if (type == GL_FLOAT_VEC3) {
//                    float val[3];
//                    glm::vec3 uniform = shader.getVec3(name);
//                    val[0] = uniform.r;
//                    val[1] = uniform.g;
//                    val[2] = uniform.b;
//                    if (ImGui::ColorButton(name, ImVec4(val[0], val[1], val[2], 1.0)))
//                        ImGui::OpenPopup("hi-picker");
//                    if (ImGui::BeginPopup("hi-picker",
//                                          ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
//                        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) ImGui::CloseCurrentPopup();
//                        ImGui::ColorPicker3("##picker", val);
//                        shader.SetVec3(name, val[0], val[1], val[2]);
//                        ImGui::EndPopup();
//                    }
//                } else if (type == GL_FLOAT_VEC2) {
//                    float val[2];
//                    glm::vec2 uniform = shader.getVe2(name);
//                    val[0] = uniform.x;
//                    val[1] = uniform.y;
//                    ImGui::DragFloat2("Vector 2", val, fspeed);
//                    shader.SetVec2(name, val[0], val[1]);
//                } else if (type == GL_FLOAT) {
//                    float val = shader.getFloat(name);
//                    ImGui::DragFloat("Float", &val, fspeed);
//                    shader.SetFloat(name, val);
//                } else if (type == GL_INT) {
//                    int val = shader.getInt(name);
//                    ImGui::DragInt("Integar", &val);
//                    shader.SetInt(name, val);
//                } else if (type == GL_BOOL) {
//                    bool val = shader.getBool(name);
//                    ImGui::Checkbox("Boolean", &val);
//                    shader.SetBool(name, val);
//                } else if (type)
//
//
//                    ImGui::NewLine();
//                ImGui::PopID();
//            }
//        }

    }


    void ComponentScript(Entity* entty) {
        ImGui::Text("script");
    }

    void ComponentDirectionalLight(Entity* entity) {
        auto& light = entity->GetComponent<DirectionalLightComponent>();
        ImGui::Text("%s", light.GetTypeName().c_str());
        float val[3];
        val[0] = light.Color.r;
        val[1] = light.Color.g;
        val[2] = light.Color.b;
        if (ImGui::ColorButton(light.GetTypeName().c_str(), ImVec4(val[0], val[1], val[2], 1.0)))
            ImGui::OpenPopup("hi-picker");
        if (ImGui::BeginPopup("hi-picker", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) ImGui::CloseCurrentPopup();
            ImGui::ColorPicker3("##picker", val);
            light.Color = {val[0], val[1], val[2]};
            ImGui::EndPopup();
        }
        ImGui::DragFloat("Strength", &light.Intensity, 0.05f, 0.0f, 1000.0f);

    }
    void ComponentPointLight(Entity* entity) {
        auto& light = entity->GetComponent<PointLightComponent>();
        ImGui::Text("%s", light.GetTypeName().c_str());
        float val[3];
        val[0] = light.Color.r;
        val[1] = light.Color.g;
        val[2] = light.Color.b;
        if (ImGui::ColorButton(light.GetTypeName().c_str(), ImVec4(val[0], val[1], val[2], 1.0)))
            ImGui::OpenPopup("hi-picker");
        if (ImGui::BeginPopup("hi-picker", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) ImGui::CloseCurrentPopup();
            ImGui::ColorPicker3("##picker", val);
            light.Color = {val[0], val[1], val[2]};
            ImGui::EndPopup();
        }
        ImGui::DragFloat("Intensity", &light.Intensity, 0.05f, 0.0f, 1000.0f);

        ImGui::DragFloat("Range", &light.Range, 0.5f, 0.0f, 100.0f);

    }
    void ComponentSpotLight(Entity* entity) {
        auto& light = entity->GetComponent<SpotLightComponent>();
        ImGui::Text("%s", light.GetTypeName().c_str());
        float val[3];
        val[0] = light.Color.r;
        val[1] = light.Color.g;
        val[2] = light.Color.b;
        if (ImGui::ColorButton(light.GetTypeName().c_str(), ImVec4(val[0], val[1], val[2], 1.0)))
            ImGui::OpenPopup("hi-picker");
        if (ImGui::BeginPopup("hi-picker", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) ImGui::CloseCurrentPopup();
            ImGui::ColorPicker3("##picker", val);
            light.Color = {val[0], val[1], val[2]};
            ImGui::EndPopup();
        }
        ImGui::DragFloat("Strength", &light.Intensity, 0.05f, 0.0f, 1000.0f);

        ImGui::DragFloat("Size", &light.Size, 0.1f, 1.0f, 180.f);
        ImGui::DragFloat("Blend", &light.Blend, 0.01f, 0.0f, 1.0f);

    }


    void PropertiesPanel::OnAttach() {


    }

    using ComponentEntry = std::pair<const char*, std::type_index>;
    // make sure you update the m_Count of the array
    std::array<ComponentEntry, 6> componentEntries = {{
        {"Transform", typeid(TransformComponent)},
        {"Script", typeid(ScriptComponent)},
        {"Mesh", typeid(MeshComponent)},

        {"Directional Light", typeid(DirectionalLightComponent)},
        {"Point Light", typeid(PointLightComponent)},
        {"Spot Light", typeid(SpotLightComponent)}
    }};
    
    // later i want the mesh components to be empty by default
    // the defauly action upon adding a new component to your entity
    std::unordered_map<std::type_index, std::function<void(Context*)>> componentActions = {
            {typeid(TransformComponent), [](Context* context){ context->m_ActiveEntity.AddComponent<TransformComponent>(); }},
            {typeid(ScriptComponent), [](Context* context){ context->m_ActiveEntity.AddComponent<ScriptComponent>(); }},
            {typeid(MeshComponent), [](Context* context){ context->m_ActiveEntity.AddComponent<MeshComponent>(MeshCube()); }},

            {typeid(DirectionalLightComponent), [](Context* context){ context->m_ActiveEntity.AddComponent<DirectionalLightComponent>(); }},
            {typeid(PointLightComponent), [](Context* context){ context->m_ActiveEntity.AddComponent<PointLightComponent>(); }},
            {typeid(SpotLightComponent), [](Context* context){ context->m_ActiveEntity.AddComponent<SpotLightComponent>(); }},
    };
    std::unordered_map<std::type_index, std::function<bool(Context*)>> componentCheckers = {
            {typeid(TransformComponent), [](Context* context) { return context->m_ActiveEntity.HasComponent<TransformComponent>(); }},
            {typeid(ScriptComponent), [](Context* context) { return context->m_ActiveEntity.HasComponent<ScriptComponent>(); }},
            {typeid(MeshComponent), [](Context* context) { return context->m_ActiveEntity.HasComponent<MeshComponent>(); }},

            {typeid(DirectionalLightComponent), [](Context* context) { return context->m_ActiveEntity.HasComponent<DirectionalLightComponent>(); }},
            {typeid(PointLightComponent), [](Context* context) { return context->m_ActiveEntity.HasComponent<PointLightComponent>(); }},
            {typeid(SpotLightComponent), [](Context* context) { return context->m_ActiveEntity.HasComponent<SpotLightComponent>(); }},
    };


    void PropertiesPanel::OnImGuiUpdate() {
        // some flags for the following
        ImGui::Begin("Properties");
        // make sure something is selected
        if (m_ActiveContext->m_ActiveEntity) {
            auto* entity = &m_ActiveContext->m_ActiveEntity;
            // the entity to use for all listed guis in the properties
            ComponentCore(entity);

            ImGui::Separator();
            ImGui::Spacing();

            // now each component will be listed below if available
            ComponentPropertyPanel<TransformComponent>(entity, ComponentTransformation, "Transform", ICON_LC_MOVE_3D);
            ComponentPropertyPanel<ScriptComponent>(entity, ComponentScript, "Script", ICON_LC_SCROLL);
            ComponentPropertyPanel<MeshComponent>(entity, ComponentMesh, "Mesh", ICON_LC_BLOCKS);

            // various light components
            ComponentPropertyPanel<DirectionalLightComponent>(entity, ComponentDirectionalLight, "Directional Light", ICON_LC_SUN);
            ComponentPropertyPanel<PointLightComponent>(entity, ComponentPointLight, "Point Light", ICON_LC_LIGHTBULB);
            ComponentPropertyPanel<SpotLightComponent>(entity, ComponentSpotLight, "Spot Light", ICON_LC_LAMP_CEILING);



            // add component button, also center it
            const char* label = "Add Component";
            float availableWidth = ImGui::GetContentRegionAvail().x;
            ImGui::SetCursorPosX((availableWidth - ImGui::CalcTextSize(label).x) / 2.0f);
            if (ImGui::Button(label)) {
                ImGui::OpenPopup("ComponentList");
            }
            if(ImGui::BeginPopup("ComponentList")) {
                for (const auto& entry : componentEntries) {
                    auto checkerIt = componentCheckers.find(entry.second);
                    if (checkerIt != componentCheckers.end() && checkerIt->second(&*m_ActiveContext)) {
                        ImGui::BeginDisabled();
                        ImGui::Selectable(entry.first);
                        ImGui::EndDisabled();
                    } else {
                        if (ImGui::Selectable(entry.first)) {
                            auto it = componentActions.find(entry.second);
                            if (it != componentActions.end()) {
                                it->second(&*m_ActiveContext);
                            }
                        }
                    }
                }
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    void PropertiesPanel::OnDetach() {

    }

    void PropertiesPanel::IndependentInput() {

    }
}