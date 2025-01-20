//
// Created by Hayden Rivas on 11/10/24.
//

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../../external/IconFontCppHeaders/IconsLucide.h"
#include "imoguizmo.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include <Slate/Components.h>
#include <Slate/Input.h>
#include <Slate/Render.h>
#include <Slate/Scope.h>


#include "../Fonts.h"
#include "../ImGuiSnippets.h"
#include "ViewportPanel.h"
#include "Slate/Debug.h"

namespace Slate {
	// post processing for view modes
	Scope<Mesh> fullscreenQuad;

	Scope<Mesh> grid;
	Scope<Mesh> quad;
	Scope<Mesh> sphere;
	Scope<Mesh> arrow;
	Scope<Mesh> spot;

	Scope<Shader> screen_depthShader;
	Scope<Shader> gridShader;
	Scope<Shader> iconShader;
	Scope<Shader> solidShader;
	Scope<Shader> outlineShader;

	Ref<Framebuffer> m_PostProcessFramebuffer;

	bool gridIsEnabled = true;
	unsigned int lightbulbTexture;
	unsigned int sunTexture;
	unsigned int lampTexture;


	void ViewportPanel::OnAttach() {
		m_ViewportCamera = CreateRef<ViewportCamera>();

		// everything required for the depth shader and its own framebuffer
		{
			FramebufferSpecification fbSpec;
			fbSpec.Attachments = {FramebufferTextureFormat::RGBA8};
			fbSpec.Width = 1280;
			fbSpec.Height = 720;
			m_PostProcessFramebuffer = CreateRef<Framebuffer>(fbSpec);


			fullscreenQuad = CreateScope<Mesh>(
					Vertices{Primitives::quadVertices2D},
					Elements{Primitives::quadIndices},
					LayoutBuffer{
							{ShaderDataType::Float2, "a_Position"},
							{ShaderDataType::Float2, "a_TexCoord"}
					}
			);
			float density = 100.0f;
			grid = CreateScope<Mesh>(
					Vertices{GenerateGridVertices(density, (unsigned int) density)},
					LayoutBuffer{{ShaderDataType::Float3, "a_Position"}},
					GL_LINES
			);

			quad = CreateScope<Mesh>(
					Vertices{Primitives::quadVertices3DNoNormals},
					Elements{Primitives::quadIndices},
					LayoutBuffer{
							{ShaderDataType::Float3, "a_Position"},
							{ShaderDataType::Float2, "a_TexCoord"}
					}
			);
			sphere = CreateScope<Mesh>(
					Vertices{GenerateSimpleSphereVertices(30)},
					LayoutBuffer{{ShaderDataType::Float3, "a_Position"}},
					GL_LINE_STRIP
			);
			arrow = CreateScope<Mesh>(
					Vertices{GenerateArrow2DMesh(2.0f, 1.0f, 0.5f, 0.8f)},
					LayoutBuffer{{ShaderDataType::Float3, "a_Position"}},
					GL_LINE_STRIP
			);
			spot = CreateScope<Mesh>(
					Vertices{GenerateSpotVertices(10)},
					LayoutBuffer{{ShaderDataType::Float3, "a_Position"}},
					GL_LINE_STRIP
			);

			stbi_set_flip_vertically_on_load(true);
			{
				// icons lightbulbTexture loading
				GL_CALL(glGenTextures(1, &lightbulbTexture));
				GL_CALL(glBindTexture(GL_TEXTURE_2D, lightbulbTexture));
				// set the lightbulbTexture wrapping/filtering options
				GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
				GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
				GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
				GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
				int width, height, nrChannels;
				unsigned char *data = stbi_load(ToDirectory("assets/textures/icons/lightbulb.png").c_str(), &width,
												&height,
												&nrChannels, 0);
				if (data) {
					GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
					GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
					GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
				} else {
					fprintf(stderr, "Failed to load lightbulbTexture\n");
				}
				GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
				stbi_image_free(data);
			}
			{
				GL_CALL(glGenTextures(1, &sunTexture));
				GL_CALL(glBindTexture(GL_TEXTURE_2D, sunTexture));
				// set the lightbulbTexture wrapping/filtering options
				GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
				GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
				GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
				GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
				int width, height, nrChannels;
				unsigned char *data = stbi_load(ToDirectory("/assets/textures/icons/sun.png").c_str(), &width, &height,
												&nrChannels, 0);
				if (data) {
					GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
					GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
					GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
				} else {
					fprintf(stderr, "Failed to load Sun Texture\n");
				}
				GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
				stbi_image_free(data);
			}
			{
				GL_CALL(glGenTextures(1, &lampTexture));
				GL_CALL(glBindTexture(GL_TEXTURE_2D, lampTexture));
				// set the lightbulbTexture wrapping/filtering options
				GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
				GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
				GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
				GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
				int width, height, nrChannels;
				unsigned char *data = stbi_load(ToDirectory("/assets/textures/icons/lamp-ceiling.png").c_str(), &width,
												&height, &nrChannels, 0);
				if (data) {
					GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
					GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
					GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
				} else {
					fprintf(stderr, "Failed to load Lamp Texture\n");
				}
				GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
				stbi_image_free(data);
			}

			// some shaders we use to visualize in the viewport, rn just depth and outline
			gridShader = CreateScope<Shader>("Grid", ToDirectory("assets/shaders/vertex/static_P.vert"),
											 ToDirectory("assets/shaders/fragment/solidcolor.frag"));
			screen_depthShader = CreateScope<Shader>("DepthFullscreen",
													 ToDirectory("assets/shaders/vertex/screen_PT.vert"),
													 ToDirectory("assets/shaders/fragment/linearize_depth.frag"));
			solidShader = CreateScope<Shader>("Solid", ToDirectory("assets/shaders/vertex/static_PT.vert"),
											  ToDirectory("assets/shaders/fragment/solidcolor.frag"));

			// requires entity input
			iconShader = CreateScope<Shader>("Icon", ToDirectory("assets/shaders/vertex/billboard_PT.vert"),
											 ToDirectory("assets/shaders/fragment/entity_icon.frag"));

		}
	}

	bool ViewportPanel::IsInViewportBounds() {
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = static_cast<int>(mx);
		int mouseY = static_cast<int>(my);

		if (mouseX >= 0 && mouseY >= 0 && mouseX < static_cast<int>(viewportSize.x) &&
			mouseY < static_cast<int>(viewportSize.y))
			return true;
		else
			return false;
	}


	bool m_CameraControlActive = false;

	void ViewportPanel::OnImGuiUpdate() {
		m_ViewportCamera->m_ActiveContext = m_ActiveContext;
		m_ViewportCamera->UpdateCamera(); // update camera right after resize for viewport to not trail

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
		{
			ImGui::PopStyleVar();
			// doesnt show in depth mode
			if (gridIsEnabled) {
				// grid mesh rendering
				GL_CALL(glEnable(GL_BLEND));
				GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
				gridShader->UseProgram();
				gridShader->SetMat4("v_ViewMatrix", m_ViewportCamera->m_ViewMatrix);
				gridShader->SetMat4("v_ProjectionMatrix", m_ViewportCamera->m_ProjectionMatrix);
				gridShader->SetMat4("v_ModelMatrix", glm::mat4(1.0f));
				grid->Bind();
				grid->DrawMesh();
				grid->Unbind();
				GL_CALL(glDisable(GL_BLEND));
			}
			for (auto id: m_ActiveContext->m_ActiveScene->GetAllIDsWith<DirectionalLightComponent, TransformComponent>()) {
				// icons for invisible (no mesh) components
				Entity entity{id, m_ActiveContext->m_ActiveScene};
				auto &transform = entity.GetComponent<TransformComponent>();
				auto &light = entity.GetComponent<DirectionalLightComponent>();

				auto model = glm::mat4(1.0f);
				model = glm::translate(model,glm::vec3(transform.Position.x, transform.Position.y, transform.Position.z));

				glm::quat rotationQuat = glm::quat(glm::radians(glm::vec3(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z)));
				auto arrowmodel = model * glm::mat4_cast(rotationQuat);


				solidShader->UseProgram();
				solidShader->SetMat4("v_ViewMatrix", m_ViewportCamera->m_ViewMatrix);
				solidShader->SetMat4("v_ProjectionMatrix", m_ViewportCamera->m_ProjectionMatrix);
				solidShader->SetMat4("v_ModelMatrix", arrowmodel);

				arrow->Bind();
				arrow->DrawMesh();
				arrow->Unbind();

				auto iconmodel = glm::scale(model, glm::vec3(0.5f));

				iconShader->UseProgram();
				iconShader->SetMat4("v_ViewMatrix", m_ViewportCamera->m_ViewMatrix);
				iconShader->SetMat4("v_ProjectionMatrix", m_ViewportCamera->m_ProjectionMatrix);
				iconShader->SetMat4("v_ModelMatrix", iconmodel);
				iconShader->SetInt("v_EntityID", static_cast<int>(id));

				iconShader->SetVec3("u_Color", light.Color.r, light.Color.g, light.Color.b);

				GL_CALL(glEnable(GL_BLEND));
				GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
				GL_CALL(glActiveTexture(GL_TEXTURE0));
				GL_CALL(glBindTexture(GL_TEXTURE_2D, sunTexture));
				quad->Bind();
				quad->DrawMesh();
				quad->Unbind();
				GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
				GL_CALL(glDisable(GL_BLEND));
			}
			// anything that can be clicked on/hovered must be drawn before the hover function below
			for (auto id: m_ActiveContext->m_ActiveScene->GetAllIDsWith<PointLightComponent, TransformComponent>()) {
				// icons for invisible (no mesh) components
				Entity entity{id, m_ActiveContext->m_ActiveScene};
				auto &transform = entity.GetComponent<TransformComponent>();
				auto &light = entity.GetComponent<PointLightComponent>();

				auto model = glm::mat4(1.0f);
				model = glm::translate(model,glm::vec3(transform.Position.x, transform.Position.y, transform.Position.z));

				auto circlemodel = glm::scale(model, glm::vec3(light.Range));

				solidShader->UseProgram();
				solidShader->SetMat4("v_ViewMatrix", m_ViewportCamera->m_ViewMatrix);
				solidShader->SetMat4("v_ProjectionMatrix", m_ViewportCamera->m_ProjectionMatrix);
				solidShader->SetMat4("v_ModelMatrix", circlemodel);

				GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
				sphere->Bind();
				sphere->DrawMesh();
				sphere->Unbind();


				auto iconmodel = glm::scale(model, glm::vec3(0.5f));

				iconShader->UseProgram();
				iconShader->SetMat4("v_ViewMatrix", m_ViewportCamera->m_ViewMatrix);
				iconShader->SetMat4("v_ProjectionMatrix", m_ViewportCamera->m_ProjectionMatrix);
				iconShader->SetMat4("v_ModelMatrix", iconmodel);
				iconShader->SetInt("v_EntityID", static_cast<int>(id));

				iconShader->SetVec3("u_Color", light.Color.r, light.Color.g, light.Color.b);


				GL_CALL(glEnable(GL_BLEND));
				GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
				GL_CALL(glActiveTexture(GL_TEXTURE0));
				GL_CALL(glBindTexture(GL_TEXTURE_2D, lightbulbTexture));
				quad->Bind();
				quad->DrawMesh();
				quad->Unbind();
				GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
				GL_CALL(glDisable(GL_BLEND));
			}

			for (auto id: m_ActiveContext->m_ActiveScene->GetAllIDsWith<SpotLightComponent, TransformComponent>()) {
				// icons for invisible (no mesh) components
				Entity entity{id, m_ActiveContext->m_ActiveScene};
				auto &transform = entity.GetComponent<TransformComponent>();
				auto &light = entity.GetComponent<SpotLightComponent>();

				auto model = glm::mat4(1.0f);
				model = glm::translate(model,glm::vec3{transform.Position.x, transform.Position.y, transform.Position.z});


				glm::quat rotationQuat = glm::quat(glm::radians(glm::vec3{transform.Rotation.x, transform.Rotation.y, transform.Rotation.z}));
				auto spotmodel = model * glm::mat4_cast(rotationQuat);


				// currently a approximation to the size of the spotlight
				// you will not as it nears 180 (the max) the model begins to not reach its fully flat state
				float radius = light.Size * 0.5f;
				spotmodel = glm::scale(spotmodel, glm::vec3{radius, 1.0f, radius});

				solidShader->UseProgram();
				solidShader->SetMat4("v_ViewMatrix", m_ViewportCamera->m_ViewMatrix);
				solidShader->SetMat4("v_ProjectionMatrix", m_ViewportCamera->m_ProjectionMatrix);
				solidShader->SetMat4("v_ModelMatrix", spotmodel);

				GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
				spot->Bind();
				spot->DrawMesh();
				spot->Unbind();


				auto iconmodel = glm::scale(model, glm::vec3(0.5f));

				iconShader->UseProgram();
				iconShader->SetMat4("v_ViewMatrix", m_ViewportCamera->m_ViewMatrix);
				iconShader->SetMat4("v_ProjectionMatrix", m_ViewportCamera->m_ProjectionMatrix);
				iconShader->SetMat4("v_ModelMatrix", iconmodel);
				iconShader->SetInt("v_EntityID", static_cast<int>(id));

				iconShader->SetVec3("u_Color", light.Color.r, light.Color.g, light.Color.b);

				GL_CALL(glEnable(GL_BLEND));
				GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
				GL_CALL(glActiveTexture(GL_TEXTURE0));
				GL_CALL(glBindTexture(GL_TEXTURE_2D, lampTexture));
				quad->Bind();
				quad->DrawMesh();
				quad->Unbind();
				GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
				GL_CALL(glDisable(GL_BLEND));
			}

			// hover entity function
			{
				auto [mx, my] = ImGui::GetMousePos();
				mx -= m_ViewportBounds[0].x;
				my -= m_ViewportBounds[0].y;
				glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
				my = viewportSize.y - my;
				int mouseX = static_cast<int>(mx);
				int mouseY = static_cast<int>(my);

				if (IsInViewportBounds()) {
					int data = m_Framebuffer->ReadPixel(1, static_cast<int>(mouseX * 2), static_cast<int>(mouseY * 2));
					m_HoveredEntity = (data == -1) ? Entity() : Entity(static_cast<entt::entity>(data),
																	   m_ActiveContext->m_ActiveScene);
				}
				// makes sure that the cursor leaving the viewport cant bring its hoverstate with it
				if (!ImGui::IsWindowHovered())
					m_HoveredEntity = Entity::Null;
			}


			// viewport camera input, don't do anything is we arent trying to use the camera
			{
				auto inputMan = SystemLocator::Get<InputManager>();
				if (inputMan.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
					// only enter camera control mode if not already active and viewport window is hovered
					if (!m_CameraControlActive && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)
						&& !ImGui::IsAnyItemActive() && !ImGui::IsAnyItemHovered()) {
						ImGui::SetWindowFocus();
						m_ViewportCamera->firstMouse = true;  // reset cursor position tracking for smooth transition
						m_CameraControlActive = true;
						inputMan.SetInputMode(GLFW_CURSOR_DISABLED);
					}
					// if already active, continue to update
					if (m_CameraControlActive) {
						m_ViewportCamera->IndependentInput();
						ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
					}
				} else if (m_CameraControlActive) {
					m_CameraControlActive = false;
					inputMan.SetInputMode(GLFW_CURSOR_NORMAL);
					ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
				}
			}

			if (ImGui::BeginMenuBar()) {

				// camera menu
				if (ImGui::BeginMenu(ICON_LC_VIDEO" Camera")) {
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
					ImGui::PushItemFlag(ImGuiItemFlags_AutoClosePopups, false);
					// DISPLAY MODES
					if (ImGui::BeginMenu("Display Modes")) {
						if (ImGui::Selectable("Shaded", m_ViewportMode == ViewportModes::SHADED))
							m_ViewportMode = ViewportModes::SHADED;
						if (ImGui::Selectable("Unshaded NW", m_ViewportMode == ViewportModes::UNSHADED))
							m_ViewportMode = ViewportModes::UNSHADED;
						if (ImGui::Selectable("Depth", m_ViewportMode == ViewportModes::DEPTH))
							m_ViewportMode = ViewportModes::DEPTH;
						if (ImGui::Selectable("Normals", m_ViewportMode == ViewportModes::NORMALS))
							m_ViewportMode = ViewportModes::NORMALS;
						if (ImGui::Selectable("Wireframe", m_ViewportMode == ViewportModes::WIREFRAME))
							m_ViewportMode = ViewportModes::WIREFRAME;
						if (ImGui::Selectable("Wireframe Color", m_ViewportMode == ViewportModes::SHADED_WIREFRAME))
							m_ViewportMode = ViewportModes::SHADED_WIREFRAME;
						ImGui::EndMenu();
					}
					ImGui::PopItemFlag();

					ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 8.0f);
					ImGui::PushItemWidth(120.f);

					ImGui::SliderFloat("Camera Speed", &m_ViewportCamera->m_CameraBaseSpeed, 1.0f, 10.0f, "%.2f");
					ImGui::SliderInt("Camera FOV", &m_ViewportCamera->m_FOV, 30.0f, 120.0f);
					ImGui::SliderFloat("Camera Far Plane", &m_ViewportCamera->m_ZFar, 1.0f, 1000.0f);
					ImGui::PopStyleVar();
					ImGui::PopItemWidth();

					ImGui::PopStyleVar();
					ImGui::EndMenu();
				}

				// environment menu
				if (ImGui::BeginMenu(ICON_LC_APERTURE" Environment")) {
					// environment color button
					ImGui::Text("Clear Color");
					ImGui::SameLine();
					auto &renderman = SystemLocator::Get<RenderManager>();

					if (ImGui::ColorButton("Clear Color", ImVec4(renderman.m_ClearColor[0], renderman.m_ClearColor[1],renderman.m_ClearColor[2], renderman.m_ClearColor[3]),
										   ImGuiColorEditFlags_NoSidePreview,
										   ImVec2(ImGui::GetFontSize() * 3, ImGui::GetTextLineHeight())))
						ImGui::OpenPopup("ev-picker");
					if (ImGui::BeginPopup("ev-picker")) {
						if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) ImGui::CloseCurrentPopup();
						ImGui::ColorPicker4("##pickerBg", reinterpret_cast<float *>(&renderman.m_ClearColor));
						ImGui::EndPopup();
					}
					// toggle viewport helpers, like grid
					if (ImGui::Button("Grid")) {
						gridIsEnabled = !gridIsEnabled;
					}
					ImGui::EndMenu();
				}
				ImGui::Separator();

				// transformations options
				if (ImGui::MenuItem(ICON_LC_MOVE, nullptr, m_GuizmoOperation == ImGuizmo::OPERATION::TRANSLATE)) {
					m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
				}
				if (ImGui::MenuItem(ICON_LC_ROTATE_CW, nullptr, m_GuizmoOperation == ImGuizmo::OPERATION::ROTATE)) {
					m_GuizmoOperation = ImGuizmo::OPERATION::ROTATE;
				}
				if (ImGui::MenuItem(ICON_LC_EXPAND, nullptr, m_GuizmoOperation == ImGuizmo::OPERATION::SCALE)) {
					m_GuizmoOperation = ImGuizmo::OPERATION::SCALE;
				}
				ImGui::Separator();
				if (m_GuizmoSpace == ImGuizmo::MODE::WORLD) {
					if (ImGui::MenuItem(ICON_LC_GLOBE " Global Space"))
						m_GuizmoSpace = ImGuizmo::MODE::LOCAL;
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled | ImGuiHoveredFlags_DelayNormal))
						ImGui::SetTooltip("Switch to Local Space");
				} else {
					if (ImGui::MenuItem(ICON_LC_FOCUS " Local Space"))
						m_GuizmoSpace = ImGuizmo::MODE::WORLD;
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled | ImGuiHoveredFlags_DelayNormal))
						ImGui::SetTooltip("Switch to Global Space");
				}

				/*       const char* label = "Diag";
					   float space_available = ImGui::GetContentRegionAvail().x;
					   if (space_available > ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x * 2) {
						   ImGui::SameLine(ImGui::GetCursorPosX() + space_available - ImGui::CalcTextSize(label).x - ImGui::GetStyle().FramePadding.x * 2);

						   if (ImGui::MenuItem(label)) {
							   // Action for Help
						   }
					   }
					   if (ImGui::MenuItem(label)) {
						   // Action for Help
					   }*/

				ImGui::EndMenuBar();
			}

			// set viewport bounds to use in other steps
			{
				auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
				auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
				auto viewportOffset = ImGui::GetWindowPos();
				m_ViewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
				m_ViewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};
			}



			// menu that determines what viewport buffer should be shown
			uint64_t textureID;
			switch (m_ViewportMode) {
				case ViewportModes::DEPTH: {
					GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

					screen_depthShader->UseProgram();
					screen_depthShader->SetFloat("u_Near", m_ViewportCamera->m_ZNear);
					screen_depthShader->SetFloat("u_Far", m_ViewportCamera->m_ZFar);
					screen_depthShader->SetFloat("u_Gamma", 2.2f);

					// will linearize the depth attachment and adjust it via gamma, it's easier to see!
					uint32_t rawDepthAttachment = m_Framebuffer->GetDepthAttachmentRendererID();
					GL_CALL(glActiveTexture(GL_TEXTURE0));
					GL_CALL(glBindTexture(GL_TEXTURE_2D, rawDepthAttachment));
					screen_depthShader->SetInt("u_DepthTexture", 0);

					// do the fullscreen rendering
					m_PostProcessFramebuffer->Bind();
					fullscreenQuad->Bind();
					fullscreenQuad->DrawMesh();
					fullscreenQuad->Unbind();
					m_PostProcessFramebuffer->Unbind();

					// now get the result and set it to be our textureID
					textureID = m_PostProcessFramebuffer->GetColorAttachmentRendererID();
					m_ActiveContext->m_ShaderMode = SHADERMODE::SOLIDWHITE;
					break;
				}
				case ViewportModes::NORMALS: {
					GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
					textureID = m_Framebuffer->GetColorAttachmentRendererID();
					m_ActiveContext->m_ShaderMode = SHADERMODE::NORMALS_ONLY;
					break;
				}
				case ViewportModes::WIREFRAME: {
					GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
					textureID = m_Framebuffer->GetColorAttachmentRendererID();
					m_ActiveContext->m_ShaderMode = SHADERMODE::SOLIDWHITE;
					break;
				}
				case ViewportModes::SHADED_WIREFRAME : {
					GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
					textureID = m_Framebuffer->GetColorAttachmentRendererID();
					m_ActiveContext->m_ShaderMode = SHADERMODE::STANDARD;
					break;
				}
				default:
					/* if (m_ActiveContext->m_ActiveEntity && !m_ViewportCamera->wireFrameEnabled) {

						 glUseProgram(outlineShader);
						 glUniform2f(glGetUniformLocation(outlineShader, "u_ScreenSize"), m_Framebuffer->GetSpecification().Width, m_Framebuffer->GetSpecification().Height);

						 uint32_t rawDepthAttachment = m_Framebuffer->GetDepthAttachmentRendererID();
						 glActiveTexture(GL_TEXTURE0);
						 glBindTexture(GL_TEXTURE_2D, rawDepthAttachment);
						 glUniform1i(glGetUniformLocation(outlineShader, "u_DepthTexture"), 0);

						 uint32_t colorAttachment = m_Framebuffer->GetColorAttachmentRendererID();
						 glActiveTexture(GL_TEXTURE1);
						 glBindTexture(GL_TEXTURE_2D, colorAttachment);
						 glUniform1i(glGetUniformLocation(outlineShader, "u_ColorTexture"), 1);

						 uint32_t stencilAttachment = m_Framebuffer->GetColorAttachmentRendererID(1);
						 glActiveTexture(GL_TEXTURE2);
						 glBindTexture(GL_TEXTURE_2D, stencilAttachment);
						 glUniform1i(glGetUniformLocation(outlineShader, "u_StencilTexture"), 2);
						 glUniform1i(glGetUniformLocation(outlineShader, "u_EntityID"), (int)(entt::entity) m_ActiveContext->m_ActiveEntity);


						 if (m_ViewportCamera->wireFrameEnabled) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						 m_PostProcessFramebuffer->Bind();
						 RenderManager::BindVertexArray(screenVAO);
						 glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
						 RenderManager::UnbindVertexArray();
						 m_PostProcessFramebuffer->Unbind();
						 if (m_ViewportCamera->wireFrameEnabled) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

						 textureID = m_PostProcessFramebuffer->GetColorAttachmentRendererID();*/
					// get the 0th (standard) color buffer and send it right away to the image


					GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
					textureID = m_Framebuffer->GetColorAttachmentRendererID();
					m_ActiveContext->m_ShaderMode = SHADERMODE::STANDARD;
					break;
			}



			// viewport image
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2 *) &viewportPanelSize) && viewportPanelSize.x > 0.0f &&
				viewportPanelSize.y > 0.0f) {
				m_ViewportCamera->OnResize(viewportPanelSize.x, viewportPanelSize.y);
				m_Framebuffer->Resize(static_cast<uint32_t>(viewportPanelSize.x * 2),
									  static_cast<uint32_t>(viewportPanelSize.y * 2));
				m_PostProcessFramebuffer->Resize(static_cast<uint32_t>(viewportPanelSize.x * 2),
												 static_cast<uint32_t>(viewportPanelSize.y * 2));
				m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
			}

			ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1},ImVec2{1, 0});

			// this used a 2d method that was lackluster, now better method in main loop
			// after image is rendered in imgui, draw text & icons on top
			/*   for (auto id : m_ActiveContext->m_ActiveScene->GetAllIDsWith<TransformComponent, TextComponent>()) {
				   Entity entity{id, m_ActiveContext->m_ActiveScene};
				   auto entityText = entity.GetComponent<TextComponent>();
				   auto entityTransform = entity.GetComponent<TransformComponent>();
				   Render3DText(entityText.Content, ImVec4{entityText.Color[0], entityText.Color[1], entityText.Color[2], entityText.Color[3]},
								entityTransform.Position, m_ViewportCamera->m_CameraPosition,
								m_ViewportCamera->m_ViewMatrix, m_ViewportCamera->m_ProjectionMatrix,
								m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
			   }*/

			// OVERLAY DEBUG MENU
			{
				ImVec2 imagePos = ImGui::GetItemRectMin(); // Top-left corner of the image
				ImVec2 textPos = ImVec2(imagePos.x + 5, imagePos.y + 5); // Adjust 5 for padding
				ImGui::SetCursorScreenPos(textPos);
				auto &io = ImGui::GetIO();
				if (m_ViewportMode == ViewportModes::DEPTH)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0, 0.0, 0.0, 1.0));
				ImGui::PushFont(Fonts::boldFont);
				ImGui::Text("ImGui io Statistics");
				ImGui::PopFont();
				ImGui::Text("Platform Name: %s", io.BackendPlatformName ? io.BackendPlatformName : "Unknown Platform");
				ImGui::Text("Backend RenderManager: %s",
							io.BackendRendererName ? io.BackendRendererName : "Unknown RenderManager");
				ImGui::Text("Capturing Mouse: %s", io.WantCaptureMouse ? "True" : "False");
				ImGui::Text("Display Size: %.1f x %.1f", io.DisplaySize.x, io.DisplaySize.y);
				ImGui::Text("Viewport Size: %.1f x %.1f", m_ViewportSize.x, m_ViewportSize.y);
				ImGui::Text("Framebuffer Resolution: %.1u x %.1u", m_Framebuffer->GetSpecification().Width,
							m_Framebuffer->GetSpecification().Height);
				ImGui::Text("Display Framebuffer Scale: %.1f x %.1f", io.DisplayFramebufferScale.x,
							io.DisplayFramebufferScale.y);
				ImGui::Text("Framerate: %.2f", io.Framerate);
				ImGui::Text("ImGui Delta Time: %.2f", io.DeltaTime);
				if (m_HoveredEntity)
					ImGui::Text("Hovered Entity: %s | %u", m_HoveredEntity.GetComponent<CoreComponent>().name.c_str(),
								static_cast<int>(m_HoveredEntity.GetEntityHandle()));
				else
					ImGui::Text("Hovered Entity: 'NONE'");
				if (m_ViewportMode == ViewportModes::DEPTH) ImGui::PopStyleColor();
			}


			// secondary gizmo (top right)
			{
				// secondary guizmo frame
				// now imOguizmo, smaller header only library but its pretty nice
				float size = ImGui::GetFontSize() * 7.0f;
				float padding = 10.0f;
				ImOGuizmo::config.lineThicknessScale = 0.04f;
				ImOGuizmo::config.axisLengthScale = 0.25f;

				ImVec4 *guizColors = *&ImGuizmo::GetStyle().Colors; // get main gizmo colors and apply it to this too
				ImOGuizmo::config.xCircleFrontColor = ImGui::ColorConvertFloat4ToU32(guizColors[ImGuizmo::DIRECTION_X]);
				ImOGuizmo::config.yCircleFrontColor = ImGui::ColorConvertFloat4ToU32(guizColors[ImGuizmo::DIRECTION_Y]);
				ImOGuizmo::config.zCircleFrontColor = ImGui::ColorConvertFloat4ToU32(guizColors[ImGuizmo::DIRECTION_Z]);

				float amountDesat = 0.5f;
				ImOGuizmo::config.xCircleBackColor = ImGui::ColorConvertFloat4ToU32(
						Desaturate(guizColors[ImGuizmo::DIRECTION_X], amountDesat));
				ImOGuizmo::config.yCircleBackColor = ImGui::ColorConvertFloat4ToU32(
						Desaturate(guizColors[ImGuizmo::DIRECTION_Y], amountDesat));
				ImOGuizmo::config.zCircleBackColor = ImGui::ColorConvertFloat4ToU32(
						Desaturate(guizColors[ImGuizmo::DIRECTION_Z], amountDesat));


				ImGui::PushFont(Fonts::boldFont);
				ImOGuizmo::SetRect(m_ViewportBounds[1].x - size - padding, m_ViewportBounds[0].y + padding, size);
				// doesnt work right now
				glm::mat4 tmpMat = m_ViewportCamera->m_ViewMatrix;
				if (ImOGuizmo::DrawGizmo(glm::value_ptr(tmpMat),
										 glm::value_ptr(m_ViewportCamera->GetProjectionMatrix()), 0.0f)) {
				}
				ImGui::PopFont();
			}


			// gizmo functionality
			if (m_ActiveContext->m_ActiveEntity && !m_CameraControlActive &&
				m_ActiveContext->m_ActiveEntity.HasComponent<TransformComponent>()) {
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				// properly set bounds as these are specific to the viewport image not just the window
				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y,
								  m_ViewportBounds[1].x - m_ViewportBounds[0].x,
								  m_ViewportBounds[1].y - m_ViewportBounds[0].y);

				// SNAP logic
				bool snap = ImGui::IsKeyDown(ImGuiKey_LeftCtrl);
				// the snap value to use for all operations but rotate
				float snapValue = 0.5f;
				if (m_GuizmoOperation == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;
				// put the snapping scale on each axis
				float snapValues[3] = {snapValue, snapValue, snapValue};

				auto &entityTC = m_ActiveContext->m_ActiveEntity.GetComponent<TransformComponent>();
				// more snapping logic to round position if i start snapping
				glm::vec3 snappedPosition = entityTC.Position;
				// ok this is scuffed but it basically works when you stop using the gizmo somehow
				if (snap && ImGuizmo::IsUsing()) {
					snappedPosition.x = roundf(snappedPosition.x / snapValue) * snapValue;
					snappedPosition.y = roundf(snappedPosition.y / snapValue) * snapValue;
					snappedPosition.z = roundf(snappedPosition.z / snapValue) * snapValue;
					entityTC.Position = snappedPosition;
				}

				float tmpMatrix[16];
				ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(snappedPosition),
														glm::value_ptr(entityTC.Rotation),
														glm::value_ptr(entityTC.Scale), tmpMatrix);

				ImGuizmo::Manipulate(glm::value_ptr(m_ViewportCamera->GetViewMatrix()),
									 glm::value_ptr(m_ViewportCamera->GetProjectionMatrix()),
									 m_GuizmoOperation, m_GuizmoSpace,
									 tmpMatrix, nullptr, snap ? snapValues : nullptr);
				// now apply the transformations
				if (ImGuizmo::IsUsing()) {
					float positon[3], rotation[3], scale[3];
					ImGuizmo::DecomposeMatrixToComponents(tmpMatrix, positon, rotation, scale);
					if (m_GuizmoOperation == ImGuizmo::OPERATION::TRANSLATE)
						entityTC.Position = glm::vec3(positon[0], positon[1], positon[2]);
					if (m_GuizmoOperation == ImGuizmo::OPERATION::ROTATE)
						entityTC.Rotation = glm::vec3(rotation[0], rotation[1], rotation[2]);
					if (m_GuizmoOperation == ImGuizmo::OPERATION::SCALE)
						entityTC.Scale = glm::vec3(scale[0], scale[1], scale[2]);
				}
			}
			// mouse interaction with clicked object must be after guizmo
			// required so that overlapping guizmo and different object dont interfere
			if (SystemLocator::Get<InputManager>().IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				if (m_HoveredEntity && !ImGuizmo::IsUsing())
					m_ActiveContext->m_ActiveEntity = m_HoveredEntity;
			}
			// right now will currently go off in menu that overlaps the image
			if (ImGui::IsMouseDoubleClicked(GLFW_MOUSE_BUTTON_LEFT) && IsInViewportBounds() && !m_HoveredEntity) {
				m_ActiveContext->m_ActiveEntity = Entity::Null;
			}
		}
		ImGui::End();
	}

	void ViewportPanel::IndependentInput() {

	}

	void ViewportPanel::OnDetach() {

	}
}