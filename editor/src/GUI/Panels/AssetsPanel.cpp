//
// Created by Hayden Rivas on 11/10/24.
//

#include "AssetsPanel.h"

#include "imgui.h"

namespace Slate {

    void AssetsPanel::OnImGuiUpdate() {
        ImGui::Begin("Asset Browser");
        {
            ImGui::Text("placeholder");
        }
        ImGui::End();
    }

    void AssetsPanel::OnAttach() {

    }

    void AssetsPanel::OnDetach() {

    }

    void AssetsPanel::IndependentInput() {

    }
}