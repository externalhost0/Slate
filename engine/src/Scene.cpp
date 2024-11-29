//
// Created by Hayden Rivas on 11/9/24.
//

// be careful when including components
#include "Slate/Components.h"
#include "Slate/Scene.h"
#include "Slate/Entity.h"

namespace Slate {

    void Scene::OnRuntimeStart() {

    }

    void Scene::OnRuntimeStop() {

    }

    void Scene::OnUpdateRuntime() {

    }
    // series of entity creations
    Entity Scene::CreateEntity() {
        return CreateEntity("Unnamed Entity");
    }
    Entity Scene::CreateEntity(const std::string &name) {
        return CreateEntity(UUID(), name);
    }
    Entity Scene::CreateEntity(UUID uuid, const std::string &name) {
        Entity entity = {m_Registry.create(), this};
        entity.ID = uuid;
        entity.AddComponent<CoreComponent>(name);

        m_EntityMap[uuid] = (entt::entity) entity;
        return entity;
    }


    template<typename... Component>
    static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src) {
        CopyComponentIfExists<Component...>(dst, src);
    }
    template<typename... Component>
    static void CopyComponentIfExists(Entity dst, Entity src) {
        ([&]() {
            if (src.HasComponent<Component>())
                dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
        }(), ...);
    }
    Entity Scene::DuplicateEntity(Entity entity) {
        // Copy name because we're going to modify component data structure
        std::string name = entity.GetComponent<CoreComponent>().name;
        Entity newEntity = CreateEntity(name + " Copy");
        CopyComponentIfExists(AllComponents{}, newEntity, entity);
        return newEntity;
    }

    void Scene::DestroyEntity(Entity entity) {
        m_EntityMap.erase(entity.GetUUID());
        m_Registry.destroy((entt::entity) entity);
    }



    // essentially requires that every type of component has their own implementation of OnComponentAdded
    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component) {
        static_assert(sizeof(T) == 0);
    }

    template<>
    void Scene::OnComponentAdded<CoreComponent>(Entity entity, CoreComponent& component)
    {

    }
    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {

    }
    template<>
    void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
    {

    }


}