//
// Created by Hayden Rivas on 11/7/24.
//

#ifndef SLATE_VIEWPORTCAMERA_H
#define SLATE_VIEWPORTCAMERA_H

#include <glm/glm.hpp>
#include "Slate/Scene.h"
#include "Slate/Ref.h"
#include "../Context.h"

#include "yaml-cpp/yaml.h"

namespace Slate {
    class ViewportCamera {
    public:
        ViewportCamera();
        explicit ViewportCamera(YAML::Node configfile);

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
        bool wireFrameEnabled = false;
        float m_CameraBaseSpeed = 5.0f;
    private:
        glm::mat4 m_ProjectionMatrix{};
        glm::mat4 m_ViewMatrix{};
        // properties
        float m_CameraActualSpeed{};
        float m_ZFar;
        float m_ZNear;
        int m_FOV;
        float m_AspectRatio{};

        void MouseUpdate();
        void ProcessMouse(float xoffset, float yoffset);

        friend class ViewportPanel;
    };

}

#endif //SLATE_VIEWPORTCAMERA_H
