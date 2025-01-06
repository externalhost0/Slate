//
// Created by Hayden Rivas on 11/9/24.
//

#pragma once

#include <entt/entt.hpp>
#include "UUID.h"

namespace Slate {
    // forward declaration, same is done in Entity.h
    class Entity;

    class Scene {
    public:
        Scene() = default;
        ~Scene() = default;

        entt::registry m_Registry;

        void OnRuntimeStart();
        void OnUpdateRuntime();
        void OnRuntimeStop();
        void SetPaused(bool paused) { m_IsPaused = paused; }

        Entity CreateEntity(); // blank
        Entity CreateEntity(const std::string& name = std::string());
        Entity CreateEntity(UUID uuid, const std::string& name = std::string());
        Entity DuplicateEntity(Entity entity);
        void DestroyEntity(Entity entity);

        template<typename... Components>
        auto GetAllIDsWith() {
            return m_Registry.view<Components...>();
        }

        template<typename... Components>
        auto GetAllEntitiesWith() {
            auto view = m_Registry.view<Components...>();
            std::vector<Entity> entity_vector;
            for (auto id : view) {
                entity_vector.emplace_back(id, this);
            }
            return entity_vector;
        }

    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);

        std::unordered_map<UUID, entt::entity> m_EntityMap;
        bool m_IsPaused = false;

        friend class Entity;
    };
}