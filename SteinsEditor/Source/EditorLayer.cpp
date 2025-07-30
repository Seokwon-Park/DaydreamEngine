#include "EditorLayer.h"
#include "imgui/imgui.h"

namespace Steins
{
	EditorLayer::EditorLayer()
		:Layer("Editor Layer")
	{

	}

	void EditorLayer::OnUpdate(Float32 _deltaTime)
	{
		Steins::RenderPassDesc rpDesc;

		Steins::RenderPassAttachmentDesc attach{};
		attach.format = Steins::RenderFormat::R8G8B8A8_UNORM;
		attach.loadOp = Steins::AttachmentLoadOp::Clear;
		attach.storeOp = Steins::AttachmentStoreOp::Store;
		rpDesc.colorAttachments.push_back(attach);

		attach.format = Steins::RenderFormat::D24_UNORM_S8_UINT;
		attach.loadOp = Steins::AttachmentLoadOp::Clear;
		attach.storeOp = Steins::AttachmentStoreOp::Store;
		rpDesc.depthAttachment = attach;

		renderPass = Steins::RenderPass::Create(rpDesc);
		renderPass->SetClearColor(Steins::Color::White);

		Steins::FramebufferDesc fbDesc;
		fbDesc.width = 320;
		fbDesc.height = 180;
		//fbDesc.width = 1600;
		//fbDesc.height = 900;

		viewportFramebuffer = Steins::Framebuffer::Create(renderPass, fbDesc);

		Steins::PipelineStateDesc desc;
		desc.vertexShader = vs;
		desc.pixelShader = ps;
		desc.renderPass = renderPass;

		pso = Steins::PipelineState::Create(desc);


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
					// Steins::Application::Get().Close(); // ���ø����̼� ���� ��û
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
		ImGui::Begin("Viewport");
		ImVec2 ImGuiViewportSize = ImGui::GetContentRegionAvail();
		if (viewportSize != Vector2(ImGuiViewportSize.x, ImGuiViewportSize.y))
		{

		}
		// ���⿡ ������ ������ ���� �̹����� ImGui::Image() �� �׸� ���Դϴ�.
		// (���� ������ �� â�� �� �ֽ��ϴ�.)
		ImGui::Text("Rendered Scene will go here!");
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

		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Steins::Application::GetInstance().isMaximzed ? ImVec2(6.0f, 6.0f) : ImVec2(0.0f, 0.0f));
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
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		ImGui::End();
	}
}
