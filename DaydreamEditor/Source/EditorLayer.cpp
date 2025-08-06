#include "EditorLayer.h"
#include "imgui/imgui.h"

namespace Daydream
{
	EditorLayer::EditorLayer()
		:Layer("Editor Layer")
	{
		camera = MakeShared<Camera>();
		
		float squareVertices[4 * 9] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,	0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		};

		float squareVertices2[4 * 9] = {
			-0.3f, -0.4f, .5f, 1.0f,0.0f,0.0f,1.0f, 0.0f,1.0f,
			 -0.3f,  0.6f, .5f,0.0f,0.0f,1.0f,1.0f, 0.0f,0.0f,
			 0.7f, 0.6f, 1.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 0.0f,
			 0.7f, -0.4f, 1.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 1.0f,
		};

		Daydream::BufferLayout layout =
		{
			{ Daydream::ShaderDataType::Float3, "a_Position", "POSITION"},
			{ Daydream::ShaderDataType::Float4, "a_Color", "COLOR"},
			{ Daydream::ShaderDataType::Float2, "a_TexCoord", "TEXCOORD"}
		};
		//squareVB = Daydream::VertexBuffer::CreateStatic(squareVertices, sizeof(squareVertices), layout.GetStride());
		squareVB = Daydream::VertexBuffer::CreateDynamic(sizeof(squareVertices), layout.GetStride());
		squareVB->SetData(squareVertices, sizeof(squareVertices));

		squareVB2 = Daydream::VertexBuffer::CreateDynamic(sizeof(squareVertices2), layout.GetStride());
		squareVB2->SetData(squareVertices2, sizeof(squareVertices2));

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		squareIB = Daydream::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		uint32_t squareIndices2[6] = { 0, 1, 2, 2, 3, 0 };
		squareIB2 = Daydream::IndexBuffer::Create(squareIndices2, sizeof(squareIndices2) / sizeof(uint32_t));


		vs = Daydream::Shader::Create("Asset/Shader/VertexShader.hlsl", Daydream::ShaderType::Vertex, Daydream::ShaderLoadMode::File);
		ps = Daydream::Shader::Create("Asset/Shader/PixelShader.hlsl", Daydream::ShaderType::Pixel, Daydream::ShaderLoadMode::File);


		Daydream::BufferLayout inputlayout = {
		{ Daydream::ShaderDataType::Float3, "a_Position", "POSITION"},
		{ Daydream::ShaderDataType::Float4, "a_Color", "COLOR"},
		{ Daydream::ShaderDataType::Float2, "a_TexCoord", "TEXCOORD"}
		};

		camera->SetPosition({ 0.0f,0.0f,-1.0f });
		cameraPos = camera->GetViewProjectionMatrix();
		viewProjMat = Daydream::ConstantBuffer::Create(sizeof(Daydream::Matrix4x4));
		viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));

		auto path = Daydream::FilePath("Asset/Texture/Checkerboard.png");
		Daydream::TextureDesc textureDesc{};
		textureDesc.bindFlags = Daydream::RenderBindFlags::ShaderResource;
		textureDesc.format = Daydream::RenderFormat::R8G8B8A8_UNORM_SRGB;
		texture = Daydream::Texture2D::Create(path, textureDesc);
		///////////////////////////////////////////////////////
		Daydream::RenderPassDesc rpDesc;

		Daydream::RenderPassAttachmentDesc attach{};
		attach.format = Daydream::RenderFormat::R8G8B8A8_UNORM;
		attach.loadOp = Daydream::AttachmentLoadOp::Clear;
		attach.storeOp = Daydream::AttachmentStoreOp::Store;
		rpDesc.colorAttachments.push_back(attach);

		attach.format = Daydream::RenderFormat::D24_UNORM_S8_UINT;
		attach.loadOp = Daydream::AttachmentLoadOp::Clear;
		attach.storeOp = Daydream::AttachmentStoreOp::Store;
		rpDesc.depthAttachment = attach;

		renderPass = Daydream::RenderPass::Create(rpDesc);
		renderPass->SetClearColor(Daydream::Color::White);

		Daydream::FramebufferDesc fbDesc;
		fbDesc.width = 320;
		fbDesc.height = 180;
		//fbDesc.width = 1600;
		//fbDesc.height = 900;

		viewportFramebuffer = Daydream::Framebuffer::Create(renderPass, fbDesc);

		Daydream::PipelineStateDesc desc;
		desc.vertexShader = vs;
		desc.pixelShader = ps;
		desc.renderPass = renderPass;

		pso = Daydream::PipelineState::Create(desc);

		material = Daydream::Material::Create(pso);
		//material = pso->CreateMaterial(); // �̰͵� ����

		material->SetTexture2D("Texture", texture);
		material->SetConstantBuffer("Camera", viewProjMat);

		activeScene = MakeShared<Scene>("MainScene");
		auto entity = activeScene->CreateGameEntity();
		entity->SetName("Test");
	}

	void EditorLayer::OnUpdate(Float32 _deltaTime)
	{
		if (Daydream::Input::GetKeyPressed(Daydream::Key::Q))
		{
			camera->SetPosition(camera->GetPosition() + Daydream::Vector3(0.0f, 0.0f, 1.0f) * _deltaTime);
			cameraPos = camera->GetViewProjectionMatrix();
			viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));
		}
		if (Daydream::Input::GetKeyPressed(Daydream::Key::E))
		{
			camera->SetPosition(camera->GetPosition() + Daydream::Vector3(0.0f, 0.0f, -1.0f) * _deltaTime);
			cameraPos = camera->GetViewProjectionMatrix();
			viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));
		}

		if (Daydream::Input::GetKeyPressed(Daydream::Key::W))
		{
			camera->SetPosition(camera->GetPosition() + Daydream::Vector3(0.0f, 1.0f, 0.0f) * _deltaTime);
			cameraPos = camera->GetViewProjectionMatrix();
			viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));
		}

		if (Daydream::Input::GetKeyPressed(Daydream::Key::A))
		{
			camera->SetPosition(camera->GetPosition() + Daydream::Vector3(-1.0f, 0.0f, 0.0f) * _deltaTime);
			cameraPos = camera->GetViewProjectionMatrix();
			viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));
		}

		if (Daydream::Input::GetKeyPressed(Daydream::Key::S))
		{
			camera->SetPosition(camera->GetPosition() + Daydream::Vector3(0.0f, -1.0f, 0.0f) * _deltaTime);
			cameraPos = camera->GetViewProjectionMatrix();
			viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));
		}

		if (Daydream::Input::GetKeyPressed(Daydream::Key::D))
		{
			camera->SetPosition(camera->GetPosition() + Daydream::Vector3(1.0f, 0.0f, 0.0f) * _deltaTime);
			cameraPos = camera->GetViewProjectionMatrix();
			viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));
		}

		DAYDREAM_CORE_INFO("{}, {}, {}", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);

		renderPass->Begin(viewportFramebuffer);

		pso->Bind();
		squareVB->Bind();
		squareIB->Bind();
		material->Bind();

		Renderer::Submit(squareIB->GetCount());

		renderPass->End();

		activeScene->Update(_deltaTime);


	}

	void EditorLayer::OnImGuiRender()
	{
		CreateDockspace();

		ImGui::ShowDemoWindow();

		// ����: ���� �޴� ��
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+N")) { /* �� �� ���� */ }
				if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) { /* �� ���� ���� */ }
				if (ImGui::MenuItem("Save Scene", "Ctrl+S")) { /* �� ���� ���� */ }
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) { /* �� �ٸ� �̸����� ���� ���� */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Exit")) {
					// Daydream::Application::Get().Close(); // ���ø����̼� ���� ��û
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {} // ��Ȱ��ȭ ����
				ImGui::EndMenu();
			}
			// �ٸ� �޴��� (View, Tools, Help ��)
			ImGui::EndMainMenuBar();
		}

		// ����: �� ����Ʈ (���� ������ ����� ���⿡ ǥ�õ� ����)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 }); // ����Ʈ â �е� ����


		ImGui::Begin("Viewport", nullptr,
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse);
		//DAYDREAM_INFO("{0}, {1}", viewportSize.x, viewportSize.y);
		UpdateViewportSize();
		ImGui::Image((ImTextureID)viewportFramebuffer->GetColorAttachmentTexture(0)->GetImGuiHandle(), ImVec2{ viewportSize.x,viewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		//ImGui::Text("Rendered Scene will go here!");
		ImGui::End();
		ImGui::PopStyleVar();


		// ����: �Ӽ� �г�
		ImGui::Begin("Properties");
		ImGui::Text("Selected Object Properties");
		// ���߿� ���õ� ��ü�� Ʈ������, ��Ƽ���� �Ӽ� ���� ǥ��
		ImGui::End();

		// ����: �� ���� ���� �г�
		ImGui::Begin("Scene Hierarchy");
		ImGui::Text("Scene Graph/Hierarchy");
		// ���߿� �� ���� ��ü���� Ʈ�� ���·� ǥ��
		ImGui::End();

		// ����: �ڻ� ������ �г�
		ImGui::Begin("Asset Browser");
		ImGui::Text("Project Assets");
		// ���߿� ���� �ý��� Ž�� �� �ڻ� �̸�����
		ImGui::End();

		// ����: �ܼ�/�α� �г�
		ImGui::Begin("Console");
		ImGui::Text("Engine Logs and Messages");
		// ������ �α� �޽����� ���
		ImGui::End();
	}

	void EditorLayer::UpdateViewportSize()
	{
		ImVec2 ImGuiViewportSize = ImGui::GetContentRegionAvail();
		bool currentActive = ImGui::IsAnyItemActive();
		//static bool isResizing = true;
		ImVec2 CurWindowSize = ImGui::GetMainViewport()->Size;
		bool isWindowResized = mainWindowSize.x != ImGui::GetMainViewport()->Size.x || mainWindowSize.y != ImGui::GetMainViewport()->Size.y;
		bool isViewportResized = viewportFramebuffer->GetWidth() != ImGuiViewportSize.x || viewportFramebuffer->GetHeight()!= ImGuiViewportSize.y;
		if (currentActive) return; // �׳� Ȱ��ȭ ���¸� �ϴ� ũ������������.
		if (isViewportResized || isWindowResized) // ������ ũ�Ⱑ ����� ���� �ٸ��ų� Imgui ������ ũ�Ⱑ framebufferũ��� �ٸ��� �������� �Ȱ���
		{
				// ���� ũ��� ī�޶� �� �����ӹ��� ������Ʈ
				// �� �������� �̹� currentContentRegionSize�� ���� ũ�⸦ ��� �ֽ��ϴ�.
				if (ImGuiViewportSize.x > 1.0f && ImGuiViewportSize.y > 1.0f)
				{

					// �������� �Ϸ� �޽��� �α�
					DAYDREAM_CORE_INFO("Viewport final resized to: {0}, {1}", ImGuiViewportSize.x, ImGuiViewportSize.y);

					Renderer::EndSwapchainRenderPass(Renderer::GetCurrentWindow());
					// D3D12Framebuffer �������� (GPU ����ȭ ���� ����)
					viewportFramebuffer->Resize(static_cast<UInt32>(ImGuiViewportSize.x), static_cast<UInt32>(ImGuiViewportSize.y));
					Renderer::BeginSwapchainRenderPass(Renderer::GetCurrentWindow());

					// ī�޶��� ����Ʈ ũ�� ������Ʈ
					// camera->SetViewportSize(currentContentRegionSize.x, currentContentRegionSize.y);

					// ���� ������ �񱳸� ���� ���� ũ�� ����
					viewportSize.x = ImGuiViewportSize.x;
					viewportSize.y = ImGuiViewportSize.y;
					mainWindowSize.x = ImGui::GetMainViewport()->Size.x;
					mainWindowSize.y = ImGui::GetMainViewport()->Size.y;
					camera->UpdateAspectRatio(ImGuiViewportSize.x, ImGuiViewportSize.y);
					viewProjMat->Update(&camera->GetViewProjectionMatrix(), sizeof(Daydream::Matrix4x4));
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
