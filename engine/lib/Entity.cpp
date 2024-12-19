//
// Created by Hayden Rivas on 11/9/24.
//

#include "Slate/Entity.h"

namespace Slate {
    Entity Entity::Null{entt::null, nullptr};
    Entity::Entity(entt::entity handle, Scene *scene) : m_EntityHandle(handle), m_Scene(scene) {}
}