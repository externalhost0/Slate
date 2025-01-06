//
// Created by Hayden Rivas on 11/10/24.
//

#pragma once

#include "Slate/Ref.h"
#include "../../Context.h"

namespace Slate {
    class Panel {
    public:
        Panel() = default;
        virtual ~Panel() = default;
        inline void LinkContext(const Ref<Context>& context) { m_ActiveContext = context; }
    protected:
        Ref<Context> m_ActiveContext;
    private:
        virtual void OnAttach() = 0;
        virtual void OnImGuiUpdate() = 0;
        virtual void OnDetach() = 0;
        // independent input only gets considered when the panel is active, or being used
        virtual void IndependentInput() = 0;

        friend class EditorGUI;
    };
}