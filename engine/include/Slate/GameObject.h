//
// Created by Hayden Rivas on 12/22/24.
//

#pragma once

#include <string>
#include "UUID.h"

namespace Slate {
    class GameObject {
    public:
        std::string m_ObjectName = "Unnamed GameObject";
        UUID m_ID;
        bool m_Enabled = true;
    protected:
        virtual void OnStart() {};
        virtual void OnUpdate() {};
        virtual void OnEnd() {};
    private:

    };
}
