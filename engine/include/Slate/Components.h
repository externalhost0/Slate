//
// Created by Hayden Rivas on 11/3/24.
//
#ifndef SLATE_COMPONENTS_H
#define SLATE_COMPONENTS_H
// my own header files
#include "Render.h"
#include "Mesh.h"
#include "Primitives.h"
#include "UUID.h"
#include "Shader.h"
#include "Application.h"
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
        bool m_IsYScalable{true};
        std::string m_ShapeName{"Unnamed Mesh"};
        std::string m_ShaderName{"null"};
        std::string m_Directory;
        std::vector<Mesh> m_Meshes;
    public:
        // every mesh requires a shader to even be shown, this is the error shader if none is recieved ??
//        Ref<Shader> GetMeshShader() { return SystemLocator::Get<RenderManager>().GetShaderManager().Get(m_ShaderName); };

        // 3 types of constructors:
        MeshComponent(std::string shadername, std::string shapename, bool i2D, const Vertices& vertices, const Elements& elements)
        : m_ShaderName(std::move(shadername)), m_ShapeName(std::move(shapename)), m_IsYScalable(i2D)
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
        : m_ShaderName(std::move(shadername)), m_ShapeName(std::move(shapename)), m_IsYScalable(i2D)
        {
            m_Meshes.emplace_back(vertices, elements, layout);
        }
        MeshComponent(std::string shadername, std::string shapename, bool i2D, const Vertices& vertices, const LayoutBuffer& layout)
        : m_ShaderName(std::move(shadername)), m_ShapeName(std::move(shapename)), m_IsYScalable(i2D)
        {
            m_Meshes.emplace_back(vertices, layout);
        }
        MeshComponent(std::string shadername, std::string shapename, bool i2D, GLint mode, const Vertices& vertices, const LayoutBuffer& layout)
        : m_ShaderName(std::move(shadername)), m_ShapeName(std::move(shapename)), m_IsYScalable(i2D)
        {
            m_Meshes.emplace_back(vertices, layout, mode);
        }
        MeshComponent(std::string shadername, std::string shapename, bool i2D, const Vertices& vertices)
                : m_ShaderName(std::move(shadername)), m_ShapeName(std::move(shapename)), m_IsYScalable(i2D)
        {
            LayoutBuffer standardlayout = {
                    {ShaderDataType::Float3, "a_Position"},
                    {ShaderDataType::Float3, "a_Normal"},
                    {ShaderDataType::Float2, "a_TexCoord"}
            };
            m_Meshes.emplace_back(vertices, standardlayout);
        }

        // imported
        MeshComponent(std::string shaderName, std::string name, bool i2D, const std::string& filePath)
        : m_ShaderName(std::move(shaderName)), m_ShapeName(std::move(name)), m_IsYScalable(i2D),
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

    inline std::vector<float> GenerateGridVertices(float size, unsigned int numLines) {
        std::vector<float> vertices; // tempy for return and loop
        // auto calc spacing
        float spacing = size / static_cast<float>(numLines);
        // lines along x and z
        for (unsigned int i = 0; i <= numLines; ++i) {
            float pos = -size / 2.0f + i * spacing;  // line position

            // x-axis
            vertices.push_back(-size / 2.0f);
            vertices.push_back(0.0f);
            vertices.push_back(pos);

            vertices.push_back(size / 2.0f);
            vertices.push_back(0.0f);
            vertices.push_back(pos);

            // z-axis
            vertices.push_back(pos);
            vertices.push_back(0.0f);
            vertices.push_back(-size / 2.0f);

            vertices.push_back(pos);
            vertices.push_back(0.0f);
            vertices.push_back(size / 2.0f);
        }
        return vertices;
    }
    inline std::vector<float> GenerateCircleVertices(int numSegments) {
        float radius = 1.0f;
        std::vector<float> vertices;
        float angleIncrement = 2.0f * (float) M_PI / (float) numSegments;

        for (int i = 0; i < numSegments; ++i) {
            float angle = (float) i * angleIncrement;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);  // circle is 2D
        }

        return vertices;
    }

    inline std::vector<float> GenerateArrow2DMesh(float shaftLength, float shaftWidth, float tipWidth, float tipHeight) {
        std::vector<float> vertices;

        // bottom vertex
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        // top vertex
        vertices.push_back(0.0f);
        vertices.push_back(shaftLength);
        vertices.push_back(0.0f);

        // triangle tip
        vertices.push_back(-tipWidth);
        vertices.push_back(shaftLength);
        vertices.push_back(0.0f);

        vertices.push_back(0.0f);
        vertices.push_back(shaftLength + tipHeight);
        vertices.push_back(0.0f);

        vertices.push_back(tipWidth);
        vertices.push_back(shaftLength);
        vertices.push_back(0.0f);

        // and back to top vertex
        vertices.push_back(0.0f);
        vertices.push_back(shaftLength);
        vertices.push_back(0.0f);

        // now again but on the z axis so we have two arrow tips!
        vertices.push_back(0.0f);
        vertices.push_back(shaftLength);
        vertices.push_back(-tipWidth);

        vertices.push_back(0.0f);
        vertices.push_back(shaftLength + tipHeight);
        vertices.push_back(0.0f);

        vertices.push_back(0.0f);
        vertices.push_back(shaftLength);
        vertices.push_back(tipWidth);

        // and back again to top vertex
        vertices.push_back(0.0f);
        vertices.push_back(shaftLength);
        vertices.push_back(0.0f);

        return vertices;
    }


    struct MeshGrid : MeshComponent {
        MeshGrid() : MeshGrid("null") {}
        explicit MeshGrid(const std::string& shaderName) : MeshGrid(shaderName, 10.f, 10) {}
        MeshGrid(const std::string& shaderName, float size, unsigned int numLines)
        : MeshComponent(shaderName, "Grid", true, GL_LINES,
                        Vertices{GenerateGridVertices(size, numLines)},
                        LayoutBuffer{ {ShaderDataType::Float3, "a_Position"} }
        ){}
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

        bool NoScale{false};

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


    class BaseLight {
    public:
        BaseLight() = default;
        virtual ~BaseLight() = default;
        virtual std::string GetTypeName() = 0;
    public:
        glm::vec3 Color{1.0f, 1.0f, 1.0f};
        float Intensity{5.0f};
    };

    struct DirectionalLightComponent : BaseLight {
        DirectionalLightComponent() {

        }
        explicit DirectionalLightComponent(glm::vec3 color) {
            Color = color;
        }
    public:
        std::string GetTypeName() override { return "Directional"; }
    public:
        glm::vec3 Direction{0.0, -1.0, 0.0};
    };

    struct PointLightComponent : BaseLight {
        PointLightComponent() {

        }
        explicit PointLightComponent(glm::vec3 color) {
            Color = color;
        }
    public:
        std::string GetTypeName() override { return "Point"; }
    public:
        float Range{8.0f};
    };

    struct SpotLightComponent : BaseLight {
        SpotLightComponent() {

        }
        explicit SpotLightComponent(glm::vec3 color) {
            Color = color;
        }
    public:
        std::string GetTypeName() override { return "Spot"; }
    public:

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
    using AllComponents =
            ComponentGroup<TransformComponent, MeshComponent, ScriptComponent,
            DirectionalLightComponent, PointLightComponent, SpotLightComponent,
            TextComponent>;
}
#endif //SLATE_COMPONENTS_H
