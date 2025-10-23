#include "EditorLayer.h"
#include "imgui/imgui.h"

#include "ImGuizmo.h"

#include "Daydream/Scene/Components/LightComponent.h"
//#include "Daydream/Scene/Components/ModelRendererComponent.h"

namespace Daydream
{
	EditorLayer::EditorLayer()
		:Layer("Editor Layer")
	{
	}

	void EditorLayer::OnAttach()
	{
		//camera = MakeShared<EditorCamera>();
		editorCamera = MakeShared<EditorCamera>();
		activeScene = MakeShared<Scene>("MainScene");


		//float squareVertices[4 * 9] = {
		//-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		//-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		// 0.5f,	0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		// 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		//};

		//float squareVertices2[4 * 9] = {
		//	-0.3f, -0.4f, .5f, 1.0f,0.0f,0.0f,1.0f, 0.0f,1.0f,
		//	 -0.3f,  0.6f, .5f,0.0f,0.0f,1.0f,1.0f, 0.0f,0.0f,
		//	 0.7f, 0.6f, 1.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 0.0f,
		//	 0.7f, -0.4f, 1.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 1.0f,
		//};

		//Daydream::BufferLayout layout =
		//{
		//	{ Daydream::ShaderDataType::Float3, "a_Position", "POSITION"},
		//	{ Daydream::ShaderDataType::Float4, "a_Color", "COLOR"},
		//	{ Daydream::ShaderDataType::Float2, "a_TexCoord", "TEXCOORD"}
		//};
		////squareVB = Daydream::VertexBuffer::CreateStatic(squareVertices, sizeof(squareVertices), layout.GetStride());
		//squareVB = VertexBuffer::CreateDynamic(sizeof(squareVertices), layout.GetStride(), 0, nullptr);
		//squareVB->SetData(squareVertices, sizeof(squareVertices));

		//uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		//squareIB = Daydream::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		editorCamera->SetPosition({ 0.0f,0.0f,-2.0f });
		viewProjMat = ConstantBuffer::Create(sizeof(Daydream::Matrix4x4));
		viewProjMat->Update(&editorCamera->GetViewProjectionMatrix(), sizeof(Daydream::Matrix4x4));

		sampler = ResourceManager::GetResource<Sampler>("LinearRepeat");

		Daydream::TextureDesc textureDesc{};
		textureDesc.width = 512;
		textureDesc.height = 512;
		textureDesc.bindFlags = Daydream::RenderBindFlags::ShaderResource;
		textureDesc.format = Daydream::RenderFormat::R8G8B8A8_UNORM;
		//texture = Daydream::Texture2D::CreateFromFile("Asset/Texture/skybox/back.jpg", textureDesc);
		//texture->SetSampler(sampler);

		//textureCube = TextureCube::Create({ "Asset/Texture/skybox/right.jpg",
		//	"Asset/Texture/skybox/left.jpg",
		//	"Asset/Texture/skybox/top.jpg",
		//	"Asset/Texture/skybox/bottom.jpg",
		//	"Asset/Texture/skybox/front.jpg",
		//	"Asset/Texture/skybox/back.jpg" },
		//	textureDesc
		//);
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;
		textureCube = TextureCube::CreateEmpty(textureDesc);

		//	textureCube = TextureCube::Create({ "Resource/NoTexture.png",
	//"Resource/NoTexture.png",
	//"Resource/NoTexture.png",
	//"Resource/NoTexture.png",
	//"Resource/NoTexture.png",
	//"Resource/NoTexture.png" },
	//		textureDesc
	//	);
		textureCube->SetSampler(sampler);

		renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass");
		gBufferRenderPass = ResourceManager::GetResource<RenderPass>("GBufferRenderPass");
		renderPass->SetClearColor(Daydream::Color::Blue);

		Daydream::FramebufferDesc fbDesc;
		fbDesc.width = 320;
		fbDesc.height = 180;
		//fbDesc.width = 1600;
		//fbDesc.height = 900;

		viewportFramebuffer = Framebuffer::Create(renderPass, fbDesc);

		fbDesc.width = 1600;
		fbDesc.height = 900;
		gBufferFramebuffer = Framebuffer::Create(gBufferRenderPass, fbDesc);


		pso = ResourceManager::GetResource<PipelineState>("SpritePSO");
		pso3d = ResourceManager::GetResource<PipelineState>("ForwardPSO");
		gBufferPSO = ResourceManager::GetResource<PipelineState>("GBufferPSO");
		skyboxPipeline = ResourceManager::GetResource<PipelineState>("CubemapPSO");
		equirectangleToCubePipeline = ResourceManager::GetResource<PipelineState>("EquirectangularPSO");
		deferredLightingPSO = ResourceManager::GetResource<PipelineState>("DeferredPSO");

		material = Material::Create(pso);
		material->SetTexture2D("Texture", texture);
		material->SetConstantBuffer("Camera", viewProjMat);

		material3d = Material::Create(pso3d);

		material3d->SetConstantBuffer("Camera", viewProjMat);

		materialcube = Material::Create(skyboxPipeline);
		materialcube->SetConstantBuffer("Camera", viewProjMat);
		materialcube->SetTextureCube("TextureCubemap", activeScene->GetSkybox()->GetSkyboxTexture());
		auto texture = ResourceManager::GetResource<Texture2D>("Asset\\Texture\\kloofendal_48d_partly_cloudy_puresky_4k.hdr");
		//materialcube->SetTexture2D("Texture", texture);

		deferredLightingMaterial = Material::Create(deferredLightingPSO);


		///////////////////////////////////////////////////////
		auto entity = activeScene->CreateGameEntity();
		entity->SetName("Test");

		activeScene->SetCurrentCamera(editorCamera);
		//Cubemap Mesh
		//auto meshData = MeshGenerator::CreateCube(5.0f);
		auto meshData = MeshGenerator::CreateSphere(100.0f, 20.0f, 20.0f);
		Array<Vector3> positions;
		for (Vertex v : meshData.vertices)
		{
			positions.push_back(v.position);
		}

		cubeVBO = VertexBuffer::CreateStatic(sizeof(Vector3) * positions.size(), 12, positions.data());
		cubeIBO = IndexBuffer::Create(meshData.indices.data(), meshData.indices.size());
		mesh = Mesh::Create(cubeVBO, cubeIBO);
		/////////////////////////////////////////////////////////////////////////////////////


		//model = MakeShared<Model>(mesh);
		model = MakeShared<Model>();
		//model->Load("Asset/Model/Lowpoly_tree_sample.fbx");
		//model->Load("Asset/Model/cerberusgun/scene.gltf");
		model->Load("Asset/Model/scene.gltf");

		//cubeIBO = IndexBuffer::Create(squareIndices2, sizeof(squareIndices2) / sizeof(uint32_t));

		ModelRendererComponent* component = entity->AddComponent<ModelRendererComponent>();
		component->SetModel(model);

		////////////////////////Light////////////////////////////
		auto lightEntity = activeScene->CreateGameEntity("Directional Light");
		lightEntity->AddComponent<LightComponent>();
		//lightBuffer = Daydream::ConstantBuffer::Create(sizeof(LightData));

		//material3d->SetConstantBuffer("Lights", lightBuffer);

		viewportPanel = MakeUnique<ViewportPanel>();
		propertyPanel = MakeUnique<PropertyPanel>();
		sceneHierarchyPanel = MakeUnique<SceneHierarchyPanel>();
		sceneHierarchyPanel->SetCurrentScene(activeScene.get());

		assetBrowserPanel = MakeUnique<AssetBrowserPanel>();
		skyboxPanel = MakeUnique<SkyboxPanel>();

		skyboxPanel->SetSkybox(activeScene->GetSkybox());
	}

	void EditorLayer::OnUpdate(Float32 _deltaTime)
	{
		editorCamera->Update(_deltaTime);
		viewProjMat->Update(&editorCamera->GetViewProjectionMatrix(), sizeof(Daydream::Matrix4x4));

		static bool isViewControlled = false;
		if (isViewportHovered && Input::GetMouseDown(Mouse::ButtonRight))
		{
			isViewControlled = true;
		}
		if (Input::GetMouseReleased(Mouse::ButtonRight))
		{
			isViewControlled = false;
		}
		if (isViewControlled)
		{
			editorCamera->ControlCameraView(_deltaTime);
		}

		//for (int i = 0; i < 4; i++)
		//{
		//	DAYDREAM_CORE_INFO("{}, {}, {}, {}", camera->GetProjectionMatrix().mat[i][0]
		//		, camera->GetProjectionMatrix().mat[i][1]
		//		, camera->GetProjectionMatrix().mat[i][2]
		//		, camera->GetProjectionMatrix().mat[i][3]);
		//}


		//pso->Bind();
		//squareVB->Bind();
		//squareIB->Bind();
		//material->Bind();

		//Renderer::Submit(squareIB->GetCount());
		gBufferRenderPass->Begin(gBufferFramebuffer);
		gBufferPSO->Bind();
		//pso3d->Bind();
		activeScene->Update(_deltaTime);
		gBufferRenderPass->End();

		renderPass->Begin(viewportFramebuffer);

		for (int i = 0; i < 4; i++)
		{
			gBufferFramebuffer->GetColorAttachmentTexture(i)->SetSampler(sampler);
		}

		deferredLightingPSO->Bind();
		deferredLightingMaterial->SetTexture2D("PositionTexture", gBufferFramebuffer->GetColorAttachmentTexture(0));
		deferredLightingMaterial->SetTexture2D("NormalTexture", gBufferFramebuffer->GetColorAttachmentTexture(1));
		deferredLightingMaterial->SetTexture2D("AlbedoTexture", gBufferFramebuffer->GetColorAttachmentTexture(2));
		deferredLightingMaterial->SetTexture2D("RMAOTexture", gBufferFramebuffer->GetColorAttachmentTexture(3));
		deferredLightingMaterial->SetTexture2D("BRDFLUT", activeScene->GetSkybox()->GetBRDF());
		deferredLightingMaterial->SetConstantBuffer("Lights", activeScene->GetLightConstantBuffer());
		deferredLightingMaterial->SetTextureCube("IrradianceTexture", activeScene->GetSkybox()->GetIrradianceTexture());
		deferredLightingMaterial->SetTextureCube("Prefilter", activeScene->GetSkybox()->GetPrefilterTexture());
		deferredLightingMaterial->Bind();
		ResourceManager::GetResource<Mesh>("Quad")->Bind();
		Renderer::Submit(6);

		//pso3d->Bind();
		//activeScene->Update(_deltaTime);

		//if (skyboxPanel->IsUsingSkybox())
		//{
		//	skyboxPipeline->Bind();
		//	mesh->Bind();
		//	materialcube->SetTextureCube("TextureCubemap", activeScene->GetSkybox()->GetSkyboxTexture());
		//	materialcube->Bind();
		//	Renderer::Submit(mesh->GetIndexCount());
		//}
		renderPass->End();
		deferredLightingMaterial->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		CreateDockspace();

		ImGui::Begin("GBufferView");
		//static int index = 0;
		//if (ImGui::Button("Position"))index = 0;
		//if (ImGui::Button("Normal"))index = 1;
		//if (ImGui::Button("Albedo"))index = 2;
		//if (ImGui::Button("AORM"))index = 3;
		for (int i = 0; i < 4; i++)
		{
			ImGui::Image((ImTextureID)gBufferFramebuffer->GetColorAttachmentTexture(i)->GetImGuiHandle(), ImVec2{ viewportSize.x / 3,viewportSize.y / 3 });
			ImGui::SameLine;
		}
		ImGui::End();
		//UI::DrawMaterialController("SkyboxTextures", materialcube.get());
		//ImGui::BeginChild("CubemapTexture");
		{
			//// 그리드 레이아웃 계산
			//float padding = 16.0f;
			//float thumbnailSize = 80.0f;
			//float cellSize = thumbnailSize + padding;
			//float panelWidth = ImGui::GetContentRegionAvail().x;
			//int columnCount = static_cast<int>(panelWidth / 200);
			//if (columnCount < 1) columnCount = 1;
			//// 테이블 기반 그리드 시작
			//if (ImGui::BeginTable("TestTable", columnCount))
			//{
			//	if (!cubeTextures.empty())
			//	{
			//		for (int i = 0; i < 6; i++)
			//		{
			//			ImGui::TableNextColumn();
			//			ImGui::Image(cubeTextures[i]->GetImGuiHandle(), ImVec2{ 200, 200 });
			//		}
			//	}
			//	ImGui::EndTable();
			//}
		}
		//ImGui::EndChild();

		//ImGui::End();

		ImGui::ShowDemoWindow();

		// 예시: 메인 메뉴 바
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+N")) { /* 새 씬 로직 */ }
				if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) { /* 씬 열기 로직 */ }
				if (ImGui::MenuItem("Save Scene", "Ctrl+S")) { /* 씬 저장 로직 */ }
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) { /* 씬 다른 이름으로 저장 로직 */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Exit")) {
					//Daydream::Application::GetInstance(); // 애플리케이션 종료 요청
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {} // 비활성화 예시
				ImGui::EndMenu();
			}
			// 다른 메뉴들 (View, Tools, Help 등)
			ImGui::EndMainMenuBar();
		}

		// 예시: 씬 뷰포트 (실제 렌더링 결과가 여기에 표시될 영역)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 }); // 뷰포트 창 패딩 제거


		ImGui::Begin("Viewport", nullptr,
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse);
		//DAYDREAM_INFO("{0}, {1}", viewportSize.x, viewportSize.y);
		UpdateViewportSize();
		isViewportFocused = ImGui::IsWindowFocused();
		isViewportHovered = ImGui::IsWindowHovered();
		Application::GetInstance().GetImGuiLayer()->BlockEvents(!isViewportFocused && !isViewportHovered);

		ImGui::Image((ImTextureID)viewportFramebuffer->GetColorAttachmentTexture(0)->GetImGuiHandle(), ImVec2{ viewportSize.x,viewportSize.y });

		GameEntity* selectedEntity = sceneHierarchyPanel->GetSelectedEntity();
		if (selectedEntity && guizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, viewportFramebuffer->GetWidth(), viewportFramebuffer->GetHeight());

			Transform& transform = selectedEntity->GetComponent<TransformComponent>()->GetTransform();
			Matrix4x4 worldMatrix = transform.GetWorldMatrix();
			ImGuizmo::Manipulate(editorCamera->GetViewMatrix().values, editorCamera->GetProjectionMatrix().values,
				(ImGuizmo::OPERATION)guizmoType, ImGuizmo::LOCAL, worldMatrix.values);

			if (ImGuizmo::IsUsing())
			{
				Vector3 translation, rotation, scale;
				Matrix4x4::Decompose(worldMatrix, translation, rotation, scale);

				transform.position = translation;
				transform.rotation = rotation;
				transform.scale = scale;
			}
		}
		//ImGui::Text("Rendered Scene will go here!");
		ImGui::End();
		ImGui::PopStyleVar();

		// 예시: 콘솔/로그 패널
		ImGui::Begin("Console");
		ImGui::Text("Engine Logs and Messages");
		// 엔진의 로그 메시지를 출력
		ImGui::End();

		sceneHierarchyPanel->OnImGuiRender();


		propertyPanel->SetSelectedEntity(selectedEntity);
		propertyPanel->OnImGuiRender();
		assetBrowserPanel->OnImGuiRender();
		skyboxPanel->OnImGuiRender();



	}

	void EditorLayer::UpdateViewportSize()
	{
		ImVec2 ImGuiViewportSize = ImGui::GetContentRegionAvail();
		bool currentActive = ImGui::IsAnyItemActive();
		//static bool isResizing = true;
		ImVec2 CurWindowSize = ImGui::GetMainViewport()->Size;
		bool isWindowResized = mainWindowSize.x != ImGui::GetMainViewport()->Size.x || mainWindowSize.y != ImGui::GetMainViewport()->Size.y;
		bool isViewportResized = viewportFramebuffer->GetWidth() != ImGuiViewportSize.x || viewportFramebuffer->GetHeight() != ImGuiViewportSize.y;
		if (currentActive) return; // 그냥 활성화 상태면 일단 크기조절 X
		if (isViewportResized) // 윈도우 크기가 저장된 값과 다르거나 Imgui 윈도우 크기가 framebuffer크기와 다르면 리사이즈 된거임
		{
			// 최종 크기로 카메라 및 프레임버퍼 업데이트
			// 이 시점에는 이미 currentContentRegionSize가 최종 크기를 담고 있습니다.
			if (ImGuiViewportSize.x > 1.0f && ImGuiViewportSize.y > 1.0f)
			{

				// 리사이즈 완료 메시지 로깅
				DAYDREAM_CORE_INFO("Viewport final resized to: {0}, {1}", ImGuiViewportSize.x, ImGuiViewportSize.y);

				//Renderer::EndSwapchainRenderPass(Renderer::GetCurrentWindow());
				//// D3D12Framebuffer 리사이즈 (GPU 동기화 로직 포함)
				//viewportFramebuffer->Resize(static_cast<UInt32>(ImGuiViewportSize.x), static_cast<UInt32>(ImGuiViewportSize.y));
				//Renderer::BeginSwapchainRenderPass(Renderer::GetCurrentWindow());
				viewportFramebuffer->RequestResize(static_cast<UInt32>(ImGuiViewportSize.x), static_cast<UInt32>(ImGuiViewportSize.y));
				gBufferFramebuffer->RequestResize(static_cast<UInt32>(ImGuiViewportSize.x), static_cast<UInt32>(ImGuiViewportSize.y));

				// 카메라의 뷰포트 크기 업데이트
				// camera->SetViewportSize(currentContentRegionSize.x, currentContentRegionSize.y);

				// 다음 프레임 비교를 위해 최종 크기 저장
				viewportSize.x = ImGuiViewportSize.x;
				viewportSize.y = ImGuiViewportSize.y;
				mainWindowSize.x = ImGui::GetMainViewport()->Size.x;
				mainWindowSize.y = ImGui::GetMainViewport()->Size.y;
				editorCamera->UpdateAspectRatio(ImGuiViewportSize.x, ImGuiViewportSize.y);
				viewProjMat->Update(&editorCamera->GetViewProjectionMatrix(), sizeof(Daydream::Matrix4x4));
			}
		}

		//isResizing = currentActive;
		//if (viewportSize != Vector2(ImGuiViewportSize.x, ImGuiViewportSize.y))
		//{
		//	Renderer::EndSwapchainFramebuffer();
		//	viewportFramebuffer->Resize(ImGuiViewportSize.x, ImGuiViewportSize.y);
		//	viewportSize = Vector2(ImGuiViewportSize.x, ImGuiViewportSize.y);
		//	camera->UpdateAspectRatio(ImGuiViewportSize.x, ImGuiViewportSize.y);
		//	viewProjMat->Update(&camera->GetViewProjectionMatrix(), sizeof(Daydream::Matrix4x4));
		//	Renderer::BeginSwapchainFramebuffer();
		//}
	}

	void EditorLayer::OnDetach()
	{
		viewportFramebuffer = nullptr;
	}

	void EditorLayer::OnEvent(Event& _event)
	{
		EventDispatcher dispatcher(_event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		//dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& _event)
	{
		if (Input::GetMousePressed(Mouse::ButtonRight))
		{
			//DAYDREAM_INFO("BLOCK EVENT");
			return true;
		}

		switch (_event.GetKeyCode())
		{
		case Key::Q:
		{
			guizmoType = -1;
			break;
		}
		case Key::W:
		{
			guizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case Key::E:
		{
			guizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case Key::R:
		{
			guizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		}
		return false;
	}

	void EditorLayer::CreateDockspace()
	{
		static bool dockspaceOpen = true;
		static bool optFullscreen = true;
		static bool optPadding = false;
		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		//ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
		if (optFullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			windowFlags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!optPadding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Daydream::Application::GetInstance().isMaximzed ? ImVec2(6.0f, 6.0f) : ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, false ? ImVec2(6.0f, 6.0f) : ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });

		ImGui::Begin("DockingSpace", &dockspaceOpen, windowFlags);
		//m_DockSpacePos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
		//STS_CORE_WARN("Dock Space Coord = {0}, {1}", m_DockSpacePos.x, m_DockSpacePos.y);
		ImGui::PopStyleColor(); // MenuBarBg
		ImGui::PopStyleVar(2);

		if (!optPadding)
			ImGui::PopStyleVar();

		if (optFullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("EngineDockingSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		ImGui::End();
	}
}
