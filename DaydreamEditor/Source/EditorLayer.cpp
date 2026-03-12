#include "EditorLayer.h"
#include "imgui/imgui.h"

#include "ImGuizmo.h"

#include "Daydream/Scene/Components/LightComponent.h"
//#include "Daydream/Scene/Components/ModelRendererComponent.h"
#include "Daydream/Scene/Components/MeshRendererComponent.h"

namespace Daydream
{
	EditorLayer::EditorLayer()
		:Layer("Editor Layer")
	{
	}

	void EditorLayer::OnAttach()
	{
		AssetManager::LoadAssetMetadataFromDirectory("Resource");
		AssetManager::LoadAssets(LoadPhase::Early);

		editorCamera = MakeShared<EditorCamera>();
		Renderer::BeginCommandList();
		activeScene = MakeShared<Scene>("MainScene");
		Renderer::EndCommandList();

		editorCamera->SetPosition({ 0.0f,0.0f,-2.0f });
		viewProjMat = ConstantBuffer::Create(sizeof(Daydream::Matrix4x4));
		viewProjMat->Update(&editorCamera->GetViewProjectionMatrix(), sizeof(Daydream::Matrix4x4));

		entityBuffer = ConstantBuffer::Create(sizeof(EntityInfo));
		info.entityID = 0;
		info.thickness = 3;

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
		depthRenderPass = ResourceManager::GetResource<RenderPass>("DepthRenderPass");
		gBufferRenderPass = ResourceManager::GetResource<RenderPass>("GBufferRenderPass");
		maskRenderPass = ResourceManager::GetResource<RenderPass>("MaskRenderPass");
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

		maskFramebuffer = Framebuffer::Create(maskRenderPass, fbDesc);

		fbDesc.width = 4096;
		fbDesc.height = 4096;
		depthFramebuffer = Framebuffer::Create(depthRenderPass, fbDesc);

		pso = ResourceManager::GetResource<PipelineState>("SpritePSO");
		pso3d = ResourceManager::GetResource<PipelineState>("ForwardPSO");
		gBufferPSO = ResourceManager::GetResource<PipelineState>("GBufferPSO");
		skyboxPipeline = ResourceManager::GetResource<PipelineState>("CubemapPSO");
		equirectangleToCubePipeline = ResourceManager::GetResource<PipelineState>("EquirectangularPSO");
		deferredLightingPSO = ResourceManager::GetResource<PipelineState>("DeferredPSO");
		depthPSO = ResourceManager::GetResource<PipelineState>("DepthPSO");
		maskPSO = ResourceManager::GetResource<PipelineState>("MaskPSO");

		material = Material::Create(pso);
		//material->SetTexture2D("Texture", texture);
		material->SetConstantBuffer("Camera", viewProjMat);

		material3d = Material::Create(pso3d);

		material3d->SetConstantBuffer("Camera", viewProjMat);

		deferredLightingMaterial = Material::Create(deferredLightingPSO);


		///////////////////////////////////////////////////////
		auto entity = activeScene->CreateGameEntity();
		entity->SetName("Test");

		auto entity2 = activeScene->CreateGameEntity();
		entity2->SetName("Test2");

		activeScene->SetCurrentCamera(editorCamera);
		//Cubemap Mesh
		//auto meshData = MeshGenerator::CreateCube(5.0f);
		auto meshData = MeshGenerator::CreateSphere(100.0f, 20, 20);
		Array<Vector3> positions;
		for (Vertex v : meshData.vertices)
		{
			positions.push_back(v.position);
		}

		cubeVBO = VertexBuffer::CreateStatic(sizeof(Vector3) * positions.size(), 12, positions.data());
		cubeIBO = IndexBuffer::Create(meshData.indices.data(), meshData.indices.size());
		cubeMesh = Mesh::Create(cubeVBO, cubeIBO);
		/////////////////////////////////////////////////////////////////////////////////////

		//cubeIBO = IndexBuffer::Create(squareIndices2, sizeof(squareIndices2) / sizeof(uint32_t));

		////model = MakeShared<Model>(mesh);
		//model = MakeShared<Model>();
		////model->Load("Asset/Model/Lowpoly_tree_sample.fbx");
		////model->Load("Asset/Model/cerberusgun/scene.gltf");
		//model->Load("Asset/Model/scene.gltf");

		ModelRendererComponent* component = entity->AddComponent<ModelRendererComponent>();
		component->SetModel(model);

		ModelRendererComponent* component2 = entity2->AddComponent<ModelRendererComponent>();
		component2->SetModel(AssetManager::GetAssetByPath<Model>("Asset/Model/cerberusgun/scene.gltf"));

		MeshRendererComponent* component3 = entity->AddComponent<MeshRendererComponent>();

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
		sceneHierarchyPanel->Update();
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
		info.entityID = 0;
		auto entity = sceneHierarchyPanel->GetSelectedEntity();
		if (entity != nullptr)
		{
			info.entityID = entity->GetHandle().GetID();
		}

		entityBuffer->Update(&info, sizeof(EntityInfo));

		activeScene->Update(_deltaTime);

		//pso->Bind();
		//squareVB->Bind();
		//squareIB->Bind();
		//material->Bind();
		Renderer::BeginRenderPass(depthRenderPass, depthFramebuffer);
		Renderer::BindPipelineState(depthPSO);
		if (activeScene->GetLightComponent())
		{
			Renderer::SetConstantBuffer("LightSpace", activeScene->GetLightComponent()->GetLight().lightViewProjectionBuffer);
		}

		for (const auto entityHandle : activeScene->GetAllEntities())
		{
			GameEntity* entity = activeScene->GetEntity(entityHandle);

			MeshRendererComponent* meshrenderer = entity->GetComponent<MeshRendererComponent>();
			if (meshrenderer)
			{
				Renderer::SetConstantBuffer("World", entity->GetComponent<TransformComponent>()->GetWorldMatrixConstantBuffer());
				auto mesh = AssetManager::GetAsset<Mesh>(meshrenderer->GetMesh());
				if (mesh)
				{
					Renderer::BindMesh(mesh);
					Renderer::DrawIndexed(mesh->GetIndexCount());
				}
			}
		}
		Renderer::EndRenderPass(depthRenderPass);


		//Renderer::Submit(squareIB->GetCount());
		Renderer::BeginRenderPass(gBufferRenderPass, gBufferFramebuffer);
		Renderer::BindPipelineState(gBufferPSO);
		Renderer::SetConstantBuffer("Camera", editorCamera->GetViewProjectionConstantBuffer());
		for (const auto entityHandle : activeScene->GetAllEntities())
		{
			GameEntity* entity = activeScene->GetEntity(entityHandle);
			MeshRendererComponent* meshRenderer = entity->GetComponent<MeshRendererComponent>();
			if (meshRenderer == nullptr) continue;
			Renderer::SetConstantBuffer("World", entity->GetComponent<TransformComponent>()->GetWorldMatrixConstantBuffer());
			Renderer::SetConstantBuffer("Entity", entity->GetEntityHandleConstantBuffer());
			auto mesh = AssetManager::GetAsset<Mesh>(meshRenderer->GetMesh());
			auto material = AssetManager::GetAsset<Material>(meshRenderer->GetMaterial());
			if (mesh && material)
			{
				Renderer::BindMesh(mesh);
				Renderer::BindMaterial(material);
				Renderer::DrawIndexed(mesh->GetIndexCount());
			}
		}
		//pso3d->Bind();
		Renderer::EndRenderPass(gBufferRenderPass);

		// selectionFramebuffer는 반드시 "검은색(0)"으로 Clear 되어 있어야 함.

		GameEntity* selectedEntity = sceneHierarchyPanel->GetSelectedEntity(); // 선택된 객체 가져오기

		Renderer::BeginRenderPass(maskRenderPass, maskFramebuffer);
		if (selectedEntity != nullptr && selectedEntity->GetComponent<MeshRendererComponent>())
		{
			MeshRendererComponent* meshRenderer = entity->GetComponent<MeshRendererComponent>();
			if (meshRenderer != nullptr)
			{
				auto mesh = AssetManager::GetAsset<Mesh>(meshRenderer->GetMesh());

				Renderer::BindPipelineState(maskPSO);
				Renderer::SetConstantBuffer("World", entity->GetComponent<TransformComponent>()->GetWorldMatrixConstantBuffer());
				Renderer::SetConstantBuffer("Camera", editorCamera->GetViewProjectionConstantBuffer());
				if (mesh)
				{
					Renderer::BindMesh(mesh);
					Renderer::DrawIndexed(mesh->GetIndexCount());
				}
			}
		}
		Renderer::EndRenderPass(maskRenderPass);

		Renderer::BeginRenderPass(renderPass, viewportFramebuffer);

		for (int i = 0; i < 5; i++)
		{
			if (i == 4)
			{
				gBufferFramebuffer->GetColorAttachmentTexture(i)->SetSampler(ResourceManager::GetResource<Sampler>("NearestRepeat"));
			}
			else
			{
				gBufferFramebuffer->GetColorAttachmentTexture(i)->SetSampler(sampler);
			}
		}
		maskFramebuffer->GetColorAttachmentTexture(0)->SetSampler(ResourceManager::GetResource<Sampler>("NearestRepeat"));

		Renderer::BindPipelineState(deferredLightingPSO);
		Renderer::SetTexture2D("PositionTexture", gBufferFramebuffer->GetColorAttachmentTexture(0));
		Renderer::SetTexture2D("NormalTexture", gBufferFramebuffer->GetColorAttachmentTexture(1));
		Renderer::SetTexture2D("AlbedoTexture", gBufferFramebuffer->GetColorAttachmentTexture(2));
		Renderer::SetTexture2D("RMAOTexture", gBufferFramebuffer->GetColorAttachmentTexture(3));
		Renderer::SetTexture2D("BRDFLUT", activeScene->GetSkybox()->GetBRDF());
		Renderer::SetTexture2D("EntityIDTexture", gBufferFramebuffer->GetColorAttachmentTexture(4));
		Renderer::SetTexture2D("OutlineTexture", maskFramebuffer->GetColorAttachmentTexture(0));
		Renderer::SetTexture2D("DepthTexture", depthFramebuffer->GetDepthAttachmentTexture());
		Renderer::SetConstantBuffer("Lights", activeScene->GetLightConstantBuffer());
		Renderer::SetConstantBuffer("EditorData", entityBuffer);
		Renderer::SetTextureCube("IrradianceTexture", activeScene->GetSkybox()->GetIrradianceTexture());
		Renderer::SetTextureCube("Prefilter", activeScene->GetSkybox()->GetPrefilterTexture());
		//deferredLightingMaterial->Bind();
		Renderer::BindMesh(ResourceManager::GetResource<Mesh>("Quad"));
		Renderer::DrawIndexed(ResourceManager::GetResource<Mesh>("Quad")->GetIndexCount());

		////pso3d->Bind();
		////activeScene->Update(_deltaTime);

		if (skyboxPanel->IsUsingSkybox())
		{
			Renderer::BindPipelineState(skyboxPipeline);
			Renderer::BindMesh(cubeMesh);
			Renderer::SetConstantBuffer("Camera", viewProjMat);
			Renderer::SetTextureCube("TextureCubemap", activeScene->GetSkybox()->GetSkyboxTexture());
			Renderer::DrawIndexed(cubeMesh->GetIndexCount());
		}
		Renderer::EndRenderPass(renderPass);
	}

	void EditorLayer::OnImGuiRender()
	{
		CreateDockspace();

		ImGui::Begin("Test");
		//static int index = 0;
		//if (ImGui::Button("Position"))index = 0;
		//if (ImGui::Button("Normal"))index = 1;
		//if (ImGui::Button("Albedo"))index = 2;
		//if (ImGui::Button("AORM"))index = 3;
		ImGui::Image(AssetManager::GetAssetByPath<Texture2D>("Resource/skybox.hdr")->GetImGuiHandle(), ImVec2{ 100,100 });

		ImGui::Image((ImTextureID)depthFramebuffer->GetDepthAttachmentTexture()->GetImGuiHandle(), ImVec2{ viewportSize.x / 3,viewportSize.y / 3 });
		//ImGui::Image((ImTextureID)AssetManager::GetAssetByPath<Texture2D>("Resource/skybox.hdr")->GetImGuiHandle(), ImVec2{ viewportSize.x / 3,viewportSize.y / 3 });

		for (int i = 0; i < 4; i++)
		{
			ImGui::Image((ImTextureID)gBufferFramebuffer->GetColorAttachmentTexture(i)->GetImGuiHandle(), ImVec2{ viewportSize.x / 3,viewportSize.y / 3 });
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
				if (ImGui::MenuItem("New Project", "Ctrl+N"))
				{
					CreateProject();
				}
				if (ImGui::MenuItem("Open Project...", "Ctrl+O")) { /* 씬 열기 로직 */ }
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
			ImGuiWindowFlags_NoScrollWithMouse // | ImGuiWindowFlags_NoTitleBar
		);
		//DAYDREAM_INFO("{0}, {1}", viewportSize.x, viewportSize.y);
		UpdateViewportSize();
		isViewportFocused = ImGui::IsWindowFocused();
		isViewportHovered = ImGui::IsWindowHovered();

		Application::GetInstance().GetImGuiLayer()->BlockEvents(!isViewportFocused && !isViewportHovered);

		switch (viewIndex)
		{
		case 0:
		{
			ImGui::Image((ImTextureID)viewportFramebuffer->GetColorAttachmentTexture(0)->GetImGuiHandle(), ImVec2{ viewportSize.x,viewportSize.y });
			break;
		}
		case 1:
		{
			ImGui::Image((ImTextureID)gBufferFramebuffer->GetColorAttachmentTexture(0)->GetImGuiHandle(), ImVec2{ viewportSize.x,viewportSize.y });
			break;
		}
		case 2:
		{
			ImGui::Image((ImTextureID)gBufferFramebuffer->GetColorAttachmentTexture(1)->GetImGuiHandle(), ImVec2{ viewportSize.x,viewportSize.y });
			break;
		}
		case 3:
		{
			ImGui::Image((ImTextureID)gBufferFramebuffer->GetColorAttachmentTexture(2)->GetImGuiHandle(), ImVec2{ viewportSize.x,viewportSize.y });
			break;
		}
		case 4:
		{
			ImGui::Image((ImTextureID)gBufferFramebuffer->GetColorAttachmentTexture(3)->GetImGuiHandle(), ImVec2{ viewportSize.x,viewportSize.y });
			break;
		}
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(AssetManager::AssetTypeToString(AssetType::Model).c_str()))
			{
				AssetHandle* pHandle = (AssetHandle*)payload->Data;
				activeScene->CreateGameEntityFromModel(*pHandle);

				//for (auto x : model->GetMeshes())
				//{
				//	DAYDREAM_CORE_INFO("{}", x.ToString());
				//}
			}
			ImGui::EndDragDropTarget();
		}

		GameEntity* selectedEntity = sceneHierarchyPanel->GetSelectedEntity();

		isGuizmoInteract = ImGuizmo::IsUsing() || ImGuizmo::IsOver();

		if (selectedEntity && guizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
			ImVec2 contentMax = ImGui::GetWindowContentRegionMax();

			float viewX = windowPos.x + contentMin.x;
			float viewY = windowPos.y + contentMin.y;
			float viewWidth = contentMax.x - contentMin.x;   // 혹은 viewportFramebuffer->GetWidth()
			float viewHeight = contentMax.y - contentMin.y;  // 혹은 viewportFramebuffer->GetHeight()

			// 탭 높이만큼 밀려난 정확한 위치에 Rect 설정
			ImGuizmo::SetRect(viewX, viewY, viewWidth, viewHeight);

			TransformComponent* transform = selectedEntity->GetComponent<TransformComponent>();
			Matrix4x4 currentWorldMat = transform->GetWorldMatrix();
			ImGuizmo::Manipulate(editorCamera->GetViewMatrix().values, editorCamera->GetProjectionMatrix().values,
				(ImGuizmo::OPERATION)guizmoType, ImGuizmo::WORLD, currentWorldMat.values);

			if (ImGuizmo::IsUsing())
			{
				Matrix4x4 newLocalMat;
				GameEntity* parent = selectedEntity->GetParent();

				if (parent != nullptr)
				{
					// 부모의 월드 행렬을 가져옴
					Matrix4x4 parentWorldMat = parent->GetComponent<TransformComponent>()->GetWorldMatrix();

					// GLM을 이용해 역행렬 계산 (내부 데이터가 glm이므로 glm::inverse 사용)
					Matrix4x4 parentInverse;
					parentInverse = parentWorldMat.GetInversed();

					// 행렬 곱셈 (Row-Major 기준: World * Inverse)
					// 앞서 구현한 operator*가 있다면: newLocalMat = currentWorldMat * parentInverse;
					// 만약 operator가 헷갈리면 안전하게 GLM으로 직접 계산:
					newLocalMat = currentWorldMat * parentInverse; // GLM은 순서가 반대 (Inv * World)
				}
				else
				{
					// 부모가 없으면 월드 행렬이 곧 로컬 행렬
					newLocalMat = currentWorldMat;
				}

				// [수정 4] 보정된 Local Matrix를 분해하여 저장
				Vector3 translation, rotationVec, scale;
				Quaternion rotationQuat;

				// Decompose 구현에 따라 다르지만, 회전은 짐벌락 방지를 위해 Quaternion으로 뽑는 게 좋습니다.
				// Matrix4x4::Decompose(newLocalMat, translation, rotationQuat, scale); 

				// 작성하신 Decompose 함수 시그니처에 맞춰 사용하세요.
				// (만약 Vector3 오일러 각도를 쓴다면 변환 필요)
				Transform::Decompose(newLocalMat, translation, rotationVec, scale);

				transform->SetTransform(translation, rotationVec, scale);
			}
		}
		//ImGui::Text("Rendered Scene will go here!");
		ImGui::End();
		ImGui::PopStyleVar();

		// 예시: 콘솔/로그 패널
		ImGui::Begin("Console");
		ImGui::Text("Engine Logs and Messages");
		ImGui::DragInt("test", &info.thickness);
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
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos(); // Includes tab bar(height 21)
		viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
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
				maskFramebuffer->RequestResize(static_cast<UInt32>(ImGuiViewportSize.x), static_cast<UInt32>(ImGuiViewportSize.y));
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

	Pair<Int32, Int32> EditorLayer::GetViewportMousePos()
	{
		auto [mouseX, mouseY] = ImGui::GetMousePos();
		mouseX -= viewportBounds[0].x;
		mouseY -= viewportBounds[0].y;
		//glm::vec2 viewportSize = viewportBounds[1] - viewportBounds[0];
		//my = viewportSize.y - my;
		return { mouseX, mouseY };
	}

	void EditorLayer::OnDetach()
	{
		viewportPanel = nullptr;
		propertyPanel = nullptr;
		sceneHierarchyPanel = nullptr;
		assetBrowserPanel = nullptr;
		skyboxPanel = nullptr;
		viewportFramebuffer = nullptr;
	}

	void EditorLayer::OnEvent(Event& _event)
	{
		EventDispatcher dispatcher(_event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
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
		case Key::D1:
		{
			viewIndex = 0;
			break;
		}
		case Key::D2:
		{
			viewIndex = 1;
			break;
		}
		case Key::D3:
		{
			viewIndex = 2;
			break;
		}
		case Key::D4:
		{
			viewIndex = 3;
			break;
		}
		case Key::D5:
		{
			viewIndex = 4;
			break;
		}



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

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& _e)
	{
		if (isViewportHovered && !isGuizmoInteract && Input::GetMouseDown(Mouse::ButtonLeft))
		{
			//DAYDREAM_INFO("{}", gBufferFramebuffer->ReadEntityHandleFromPixel(GetViewportMousePos()));
			int test = gBufferFramebuffer->ReadEntityHandleFromPixel(GetViewportMousePos());
			if (test != 0)
			{
				sceneHierarchyPanel->SetSelectedEntity(activeScene->GetEntity(EntityHandle(test)));
			}
		}
		//DAYDREAM_INFO("Mouse Coord = {0}, {1}", GetViewportMousePos().first, GetViewportMousePos().second);

		return false;
	}

	void EditorLayer::CreateProject()
	{
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
