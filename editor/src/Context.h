//
// Created by Hayden Rivas on 11/12/24.
//

#ifndef SLATEEDITOR_CONTEXT_H
#define SLATEEDITOR_CONTEXT_H

#include <Slate/Entity.h>
//#include "Slate/Scene.h"

namespace Slate {
    class Scene;
    class Entity;
    struct Context {
        Entity m_ActiveEntity;
        Scene* m_ActiveScene;
    };
}
#endif //SLATEEDITOR_CONTEXT_H
