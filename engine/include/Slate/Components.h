//
// Created by Hayden Rivas on 11/3/24.
//
#ifndef SLATE_COMPONENTS_H
#define SLATE_COMPONENTS_H
// my own header files
#include "Renderer.h"
#include "Mesh.h"
#include "Primitives.h"
#include "UUID.h"
#include "Shader.h"
// not my own
#include <glm/glm.hpp>
#include <string>
#include <typeindex>
#include <utility>
#include <filesystem>
#include <assimp/scene.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


// remember components should only be written if they have ui manipulated or presented elements!!
namespace Slate {

    class MeshComponent {
    public:
        bool m_IsZScalable{true};
        std::string m_ShapeName{"Unnamed Mesh"};
        std::string m_ShaderName{"null"};
        std::string m_Directory;
        std::vector<Mesh> m_Meshes;
    public:
        // every mesh requires a shader to even be shown, this is the error shader if none is recieved ??
        Ref<Shader> GetMeshShader() const { return Renderer::GetShaderManager().Get(m_ShaderName); };

        // 3 types of constructors:
        MeshComponent(std::string shadername, std::string shapename, bool i2D, const Vertices& vertices, const Elements& elements)
        : m_ShaderName(std::move(shadername)), m_ShapeName(std::move(shapename)), m_IsZScalable(i2D)
        {
            // standard layout for most things, could add more data if necessary
            LayoutBuffer standardlayout = {
                    {ShaderDataType::Float3, "a_Position"},
                    {ShaderDataType::Float3, "a_Normal"},
                    {ShaderDataType::Float2, "a_TexCoord"}
            };
            m_Meshes.emplace_back(vertices, elements, standardlayout);
        }
        MeshComponent(std::string shadername, std::string shapename, bool i2D, const Vertices& vertices, const Elements& elements, const LayoutBuffer& layout)
        : m_ShaderName(std::move(shadername)), m_ShapeName(std::move(shapename)), m_IsZScalable(i2D)
        {
            m_Meshes.emplace_back(vertices, elements, layout);
        }

        // imported
        MeshComponent(std::string shaderName, std::string name, bool i2D, const std::string& filePath)
        : m_ShaderName(std::move(shaderName)), m_ShapeName(std::move(name)), m_IsZScalable(i2D),
        m_Directory(filePath)
        {
            loadModel(filePath);
        }
    private:
        void loadModel(const std::string& path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    };

    struct MeshCube : MeshComponent {
        // for adding via component call
        MeshCube() : MeshCube("null") {}
        explicit MeshCube(const std::string& shaderName)
        : MeshComponent(shaderName, "Cube", false,
                {Primitives::cubeVertices, sizeof (Primitives::cubeVertices)},
                {Primitives::cubeIndices, sizeof (Primitives::cubeIndices)}
        ) {}
    };
    struct MeshPlane : MeshComponent {
        // for adding via component call
        MeshPlane() : MeshPlane("null") {}
        explicit MeshPlane(const std::string& shaderName)
        : MeshComponent(shaderName, "Plane", false,
                {Primitives::planeVertices, sizeof (Primitives::planeVertices)},
                {Primitives::planeIndices, sizeof (Primitives::planeIndices)}
        ) {}
    };
    struct MeshQuad : MeshComponent {
        MeshQuad() : MeshQuad("null") {}
        explicit MeshQuad(const std::string& shaderName)
        : MeshComponent(shaderName, "Quad", true,
                {Primitives::quadVertices3D, sizeof (Primitives::quadVertices3D)},
                {Primitives::quadIndices, sizeof (Primitives::quadIndices)},
                {
                        {ShaderDataType::Float3, "a_Position"},
                        {ShaderDataType::Float2, "a_TexCoord"}
                }
        ) {}
    };

    // the way we import meshes is this
    struct MeshImport : MeshComponent {
        // for adding via component call???
        explicit MeshImport(const std::string& filePath) : MeshImport("null", filePath) {}
        MeshImport(const std::string& shaderName, const std::string& filePath)
        : MeshComponent(shaderName, "Object", false, filePath) {}
    };



    struct TransformComponent {
        glm::vec3 Position;
        glm::vec3 Rotation;
        glm::vec3 Scale;


        TransformComponent() : TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)) {};
        explicit TransformComponent(glm::vec3 ipos) : TransformComponent(ipos, glm::vec3(0.0f), glm::vec3(1.0f)) {};  // single arg constructor as explicit
        TransformComponent(glm::vec3 ipos, glm::vec3 irot) : TransformComponent(ipos, irot, glm::vec3(1.0f)) {};
        TransformComponent(glm::vec3 ipos, glm::vec3 irot, glm::vec3 iscal) : Position(ipos), Rotation(irot), Scale(iscal) {};

        glm::mat4 GetTransform() const {
            return glm::translate(glm::mat4(1.0f), Position)
                   * glm::toMat4(glm::quat(Rotation))
                   * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    enum class LightType {
        Ambient,
        Directional,
        Point,
        Spot
    };

    class LightComponent {
    public:
        float Strength;
        glm::vec3 Color;
        // default light component
        LightComponent() : Type(LightType::Point), Strength(1.0f), Color({1.0f, 1.0f, 1.0f}) {}
        std::string GetTypeName() const {
            switch (Type) {
                case LightType::Ambient: return "Ambient";
                case LightType::Directional: return "Directional";
                case LightType::Point: return "Point";
                case LightType::Spot: return "Spot";
                default: return "Unknown";
            }
        }
    protected:
        LightType Type;


        explicit LightComponent(LightType type) : Type(type), Strength(1.0f), Color({1.0f, 1.0f, 1.0f}) {}
    };
    struct AmbientLight : LightComponent {
        AmbientLight() : LightComponent(LightType::Ambient) {}
    };
    struct DirectionalLight : LightComponent {
        DirectionalLight() : LightComponent(LightType::Directional) {}
    };
    struct PointLight : LightComponent {
        PointLight() : LightComponent(LightType::Point) {}
    };
    struct SpotLight : LightComponent {
        SpotLight() : LightComponent(LightType::Spot) {}
    };

    struct TextComponent {
        std::string Content;
        std::array<float, 4> Color{1.0f, 0.0f, 0.0f, 1.0f};

        explicit TextComponent(std::string content)
        : Content(std::move(content)) {}

    };

    struct ScriptComponent {
        std::filesystem::path filePath;
    };


    // required for every single entity
    struct CoreComponent {
        std::string name = "Unnamed Entity";

        explicit CoreComponent(std::string name) {
            this->name = std::move(name);
        }
    };

// component just used to delete the entity that has it on a new frame
    struct MarkedForDeletion {};

    template<typename... Component>
    struct ComponentGroup{};
    using AllComponents = ComponentGroup<TransformComponent, MeshComponent, ScriptComponent>;
}
#endif //SLATE_COMPONENTS_H
