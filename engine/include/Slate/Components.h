//
// Created by Hayden Rivas on 11/3/24.
//
#ifndef SLATE_COMPONENTS_H
#define SLATE_COMPONENTS_H

// my own header files
#include "Files.h"
#include "Mesh.h"
#include "Primitives.h"
#include "UUID.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <string>
#include <typeindex>
#include <utility>
#include <assimp/scene.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>



// remember components should only have ui manipulated or presented elements!!
namespace Slate {
    // MESH COMPONENT IS IN TWO SEPERATE COMPONENTS
    class MeshComponent {
    public:
        bool m_IsZScalable{false};
        std::string m_ShapeName{"Unnamed Mesh"};
        // every mesh requires a shader to even be shown, this is the error shader if none is recieved ??
        Shader m_Shader;
    };

    // FOR PRE-DEFINED MESHES
    struct PrimitiveComponent : public MeshComponent {
        Mesh m_Mesh;
        // STATIC
        PrimitiveComponent(const Shader& ishader, const float *verts, size_t vertCount, const unsigned int *indices, size_t indiceCount, bool i2D, const std::string& iname)
        : m_Mesh(verts, vertCount, indices, indiceCount)
        {
            this->m_Shader = ishader;
            this->m_IsZScalable = i2D;
            this->m_ShapeName = iname;
        }
    };
    // FOR USER IMPORTED MESHES
    struct ModelComponent : public MeshComponent {
        std::vector<Mesh> m_Meshes;
        std::string m_Directory;
        // DYNAMIC
        ModelComponent(const Shader& ishader, const std::string& filePath, bool i2D, const std::string& iname)
        : m_Directory(filePath)
        {
            this->m_Shader = ishader;
            this->m_IsZScalable = i2D;
            this->m_ShapeName = iname;

            m_Directory = filePath;
            // construction and assignment for m_Meshes
            loadModel(filePath);
        }
    private:
        // functions that occur in the constructor
        void loadModel(const std::string& path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    };


    // some mesh primitives i supply to the editor by default
    struct MeshCube : PrimitiveComponent {
        // for adding via component call
        MeshCube() : MeshCube(Shader("../editor/assets/shaders/static.vert", "../editor/assets/shaders/basic.frag")) {}
        explicit MeshCube(const Shader& shader)
        : PrimitiveComponent(shader, cubeVertices, 64, cubeIndices, 36, false, "Cube") {}
    };
    struct MeshPlane : PrimitiveComponent {
        // for adding via component call
        MeshPlane() : MeshPlane(Shader("../editor/assets/shaders/static.vert", "../editor/assets/shaders/basic.frag")) {}
        explicit MeshPlane(const Shader& shader)
        : PrimitiveComponent(shader, planeVertices, 32, planeIndices, 6, true, "Plane") {}
    };
    // the way we import meshes is this
    struct MeshImport : ModelComponent {
        explicit MeshImport(const std::string& filePath) : MeshImport(Shader("../editor/assets/shaders/static.vert", "../editor/assets/shaders/basic.frag"), filePath) {}
        MeshImport(const Shader& shader, const std::string& filePath)
        : ModelComponent(shader, filePath, false, "Object") {}
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


    struct ScriptComponent {
        std::string filePath;
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
    using AllComponents = ComponentGroup<TransformComponent, PrimitiveComponent, ModelComponent, ScriptComponent>;
}
#endif //SLATE_COMPONENTS_H
