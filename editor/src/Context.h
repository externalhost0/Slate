//
// Created by Hayden Rivas on 11/12/24.
//

#pragma once

#include <Slate/Entity.h>
#include <Slate/Enums.h>

namespace Slate {
    class Scene;
    class Entity;

    // things the app should be aware of at anytime
    // perhaps this should be static?
    struct Context {
        Entity m_ActiveEntity;
        Scene* m_ActiveScene;
        SHADERMODE m_ShaderMode{SHADERMODE::STANDARD};
    };
}
