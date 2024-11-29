//
// Created by Hayden Rivas on 11/7/24.
//
#include <glm/gtc/matrix_transform.hpp>
#include <Slate/Components.h>
#include <Slate/Input.h>
#include <Slate/Time.h>
#include <imgui.h>

#include "ViewportCamera.h"
namespace Slate {

    ViewportCamera::ViewportCamera() :
        m_AspectRatio(1.33f),
        m_FOV(65),
        m_ZNear(0.1f),
        m_ZFar(100.f) {}


    ViewportCamera::ViewportCamera(YAML::Node configfile) :
        m_FOV(configfile["fov"].as<int>(65)),
        m_ZNear(configfile["near"].as<float>(0.1f)),
        m_ZFar(configfile["far"].as<float>(100.f)),
        m_CameraBaseSpeed(configfile["base_speed"].as<float>(5.f)) {}

    // when our viewport resizes
    void ViewportCamera::OnResize(float width, float height) {
        if (width/height == 0) m_AspectRatio = 1.33;
        m_AspectRatio = width / height;
    }

    void ViewportCamera::UpdateCamera() {
        // Set perspective projection matrix
        m_ProjectionMatrix = glm::perspective(glm::radians((float) m_FOV), m_AspectRatio, m_ZNear, m_ZFar);
        m_ViewMatrix = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraFront, cameraUp);


        // Set uniform matrices for camera specific input
        // so.. small bug where shader is shared between primitive comp and model comp so these view and proj are set on startup
        // figure a way to cache and store shaders so that we dont accidently set shaders across different components
        // look up uniform buffer objects or ssbo
        auto viewer = m_ActiveContext->m_ActiveScene->GetAllEntitiesWith<TransformComponent, MeshComponent>();
        for (auto id : viewer) {
            Entity entity {id, m_ActiveContext->m_ActiveScene};
            Renderer::GetShaderManager().Get(entity.GetComponent<MeshComponent>().m_ShaderName)->setMat4("v_ViewMatrix", m_ViewMatrix);
            Renderer::GetShaderManager().Get(entity.GetComponent<MeshComponent>().m_ShaderName)->setMat4("v_ProjectionMatrix", m_ProjectionMatrix);
            Renderer::GetShaderManager().Get(entity.GetComponent<MeshComponent>().m_ShaderName)->setVec3("v_ViewPos",  m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);

            Renderer::GetShaderManager().Get("solid_color")->setMat4("v_ViewMatrix", m_ViewMatrix);
            Renderer::GetShaderManager().Get("solid_color")->setMat4("v_ProjectionMatrix", m_ProjectionMatrix);
        }
    }

    bool isInViewport = false;
    void onKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        // quick swap into viewport
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            isInViewport = !isInViewport;
            // makes it so last position is returnable
//            firstMouse = true;
            // update the appearance of our cursor
            glfwSetInputMode(window, GLFW_CURSOR,isInViewport ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            // also change whether the gui will detect our hidden mouse
            if (isInViewport) {
                ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
            } else {
                ImGui::GetIO().ConfigFlags -= ImGuiConfigFlags_NoMouse;
            }
        }
    }

    float yaw;
    float pitch;
    void ViewportCamera::ProcessMouse(float xoffset, float yoffset) {
        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // switched for proper alignment on startup against z axis
        glm::vec3 direction;
        direction.z = -cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
        direction.y = sinf(glm::radians(pitch));
        direction.x = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));

        direction = glm::normalize(direction);
        this->m_CameraFront = direction;
    }

    double lastX;
    double lastY;
    double xpos;
    double ypos;
    const float MOUSE_SENSITIVITY = 0.1f;
    void ViewportCamera::MouseUpdate() {
        glm::vec2 pos = Input::GetMousePosition();
        xpos = pos.x;
        ypos = pos.y;

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        xoffset *= MOUSE_SENSITIVITY;
        yoffset *= MOUSE_SENSITIVITY;

        ProcessMouse((float) xoffset, (float) yoffset);
    }

    float BASE_SPEED = 5.0f;
    float SPRINT_MULTIPLIER = 2.0f;
    void ViewportCamera::IndependentInput() {
        MouseUpdate();

        glm::vec3 scaledVector;
        // sprint function in viewport camera
        if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
            m_CameraActualSpeed = m_CameraBaseSpeed * SPRINT_MULTIPLIER;
        else
            m_CameraActualSpeed = m_CameraBaseSpeed;
        float adjustedSpeed = m_CameraActualSpeed * (float) Time::GetDeltaTime();

        if (Input::IsKeyPressed(GLFW_KEY_W))
            m_CameraPosition = m_CameraPosition + adjustedSpeed * m_CameraFront;
        if (Input::IsKeyPressed(GLFW_KEY_S))
            m_CameraPosition = m_CameraPosition - adjustedSpeed * m_CameraFront;
        if (Input::IsKeyPressed(GLFW_KEY_A))
            m_CameraPosition = m_CameraPosition - glm::normalize(glm::cross(m_CameraFront, cameraUp)) * adjustedSpeed;
        if (Input::IsKeyPressed(GLFW_KEY_D))
            m_CameraPosition = m_CameraPosition + glm::normalize(glm::cross(m_CameraFront, cameraUp)) * adjustedSpeed;
        if (Input::IsKeyPressed(GLFW_KEY_SPACE))
            m_CameraPosition = m_CameraPosition + adjustedSpeed * cameraUp;
        if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
            m_CameraPosition = m_CameraPosition - adjustedSpeed * cameraUp;
    }
}