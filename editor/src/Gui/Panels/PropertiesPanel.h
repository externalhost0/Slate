//
// Created by Hayden Rivas on 11/10/24.
//

#pragma once

#include "Panel.h"

namespace Slate {
    class PropertiesPanel : public Panel {
        void OnAttach() override;
        void OnImGuiUpdate() override;
        void OnDetach() override;
        void IndependentInput() override;
    public:
        PropertiesPanel() = default;
    };
}