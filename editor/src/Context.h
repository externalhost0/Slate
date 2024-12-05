//
// Created by Hayden Rivas on 11/12/24.
//

#ifndef SLATEEDITOR_CONTEXT_H
#define SLATEEDITOR_CONTEXT_H

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
        SHADERMODE m_ShaderMode{STANDARD};
    };
}
#endif //SLATEEDITOR_CONTEXT_H
