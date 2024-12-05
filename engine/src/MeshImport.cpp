//
// Created by Hayden Rivas on 11/21/24.
//

#include <iostream>
#include <glm/vec2.hpp>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Slate/Components.h"
#include "Slate/Expect.h"

namespace Slate {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
    };
    std::vector<float> FlattenVertices(const std::vector<Vertex>& vertices) {
        std::vector<float> flatData;
        flatData.reserve(vertices.size() * (3 + 3 + 2)); // reserve space for position, normal, and texCoord

        for (const auto& vertex : vertices) {
            // Add Position
            flatData.push_back(vertex.Position.x);
            flatData.push_back(vertex.Position.y);
            flatData.push_back(vertex.Position.z);

            // Add Normal
            flatData.push_back(vertex.Normal.x);
            flatData.push_back(vertex.Normal.y);
            flatData.push_back(vertex.Normal.z);

            // Add TexCoord
            flatData.push_back(vertex.TexCoord.x);
            flatData.push_back(vertex.TexCoord.y);
        }

        return flatData;
    }

    void MeshComponent::loadModel(const std::string& path) {
        Assimp::Importer import;
        // theres more assimp flags you should read about online
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            EXPECT(false, "Assimp broke :(");
            return;
        }

        processNode(scene->mRootNode, scene); // pass the meshes vector for accumulation
    }

    // recursive call chain is not clang-tidy, cmon joey!
    void MeshComponent::processNode(aiNode *node, const aiScene *scene) {
        // process all the node's meshes (if any)
        for(unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for(unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh MeshComponent::processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
//        std::vector<Texture> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex{};
            glm::vec3 vector;
            // process positions, normals and tex coordinates
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            // if mesh has texture coordinates perform assignment
            if(mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoord = vec;
            }
            else
                vertex.TexCoord = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // process indices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process material
        /*if(mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                               aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            vector<Texture> specularMaps = loadMaterialTextures(material,
                                                                aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }*/

        LayoutBuffer layout = {
                {ShaderDataType::Float3, "a_Position"},
                {ShaderDataType::Float3, "a_Normal"},
                {ShaderDataType::Float2, "a_TexCoord"}
        };
        return {Vertices(FlattenVertices(vertices)), Elements(indices), layout};
    }
}