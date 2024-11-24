//
// Created by Hayden Rivas on 11/10/24.
//

#ifndef SLATE_SCENEPANEL_H
#define SLATE_SCENEPANEL_H

#include "Slate/Entity.h"

#include "Panel.h"

namespace Slate {
    class ScenePanel : public Panel {
        void OnAttach() override;
        void OnImGuiUpdate() override;
        void OnDetach() override;
        void IndependentInput() override;

    public:
        ScenePanel() = default;
//        Entity m_SelectedEntity;

    };
}

#endif //SLATE_SCENEPANEL_H
