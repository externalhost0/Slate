//
// Created by Hayden Rivas on 11/9/24.
//



#ifndef SLATE_ENTITY_H
#define SLATE_ENTITY_H

#include <entt/entt.hpp>

#include "UUID.h"
#include "Scene.h"

// lets find a better way of doing this later
// maybe putting it in its own class
#include "../../src/errors.h"

namespace Slate {
    class Entity {
    public:
        // construction
        Entity() : m_EntityHandle(entt::null), m_Scene(nullptr){}
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        UUID ID{};
        UUID GetUUID() const { return ID; }

        static Entity Null; // way to compare entity as null, trying to be analogous to entt::null

        // comparison operators
        bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle; }
        explicit operator bool() const { return m_EntityHandle != entt::null && this != &Entity::Null; }
        explicit operator uint32_t() const { return (uint32_t) m_EntityHandle; }

        // explict requires that we cast it later, may be good or bad idk but itst just more work
        explicit operator entt::entity() const { return m_EntityHandle; }



        // general ecs functionality
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            EXPECT(!HasComponent<T>(), "Entity already has that component!");
            T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
            m_Scene->OnComponentAdded<T>(*this, component);
            return component;
        }

        template<typename T, typename... Args>
        T& AddOrReplaceComponent(Args&&... args)
        {
            T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
            m_Scene->OnComponentAdded<T>(*this, component);
            return component;
        }

        template<typename T>
        T& GetComponent() {
            EXPECT(HasComponent<T>(), "Entity does not have that component!");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent() {
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent() {
            EXPECT(HasComponent<T>(), "Entity does not have that component!");
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

    private:
        entt::entity m_EntityHandle{entt::null};
        Scene* m_Scene = nullptr;
    };
}

#endif //SLATE_ENTITY_H
