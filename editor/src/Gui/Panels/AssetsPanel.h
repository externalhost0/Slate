//
// Created by Hayden Rivas on 11/10/24.
//

#ifndef SLATE_ASSETSPANEL_H
#define SLATE_ASSETSPANEL_H

#include "Panel.h"

namespace Slate {
    class AssetsPanel : public Panel {
        void OnAttach() override;
        void OnImGuiUpdate() override;
        void OnDetach() override;

        void IndependentInput() override;
    };
}


#endif //SLATE_ASSETSPANEL_H
