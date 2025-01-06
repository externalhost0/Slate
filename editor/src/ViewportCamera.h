//
// Created by Hayden Rivas on 11/7/24.
//

#pragma once

#include <glm/glm.hpp>
#include "Slate/Scene.h"
#include "Slate/Ref.h"
#include "Context.h"


namespace Slate {
    class ViewportCamera {
    public:

        glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; };
        glm::mat4& GetViewMatrix() { return m_ViewMatrix; };

        // unlike a camera component or camera a user would place,
        // the editor camera (personal prefrence) needs to be distinct and not follow that pattern
        glm::vec3 m_CameraPosition{0.0f, 0.0f, 5.0f}; // dynamic
        glm::vec3 m_CameraFront{0.0f, 0.0f, -1.0f};    // dynamic
        glm::vec3 cameraUp{0.0f, 1.0f, 0.0f};      // static (up will always be straight up)

        // kinda important so it knows what to render
        Ref<Context> m_ActiveContext;

        // public methods
        void UpdateCamera();
        void IndependentInput();
        void OnResize(float width, float height);

        bool firstMouse = true;
        float m_CameraBaseSpeed = 5.0f;
    private:
        glm::mat4 m_ProjectionMatrix{};
        glm::mat4 m_ViewMatrix{};
        // properties
        float m_CameraActualSpeed{};
        float m_ZFar{100.0f};
        float m_ZNear{1.f};
        int m_FOV{65};
        float m_AspectRatio{1.33f};

        void MouseUpdate();
        void ProcessMouse(float xoffset, float yoffset);

        friend class ViewportPanel;
    };

}