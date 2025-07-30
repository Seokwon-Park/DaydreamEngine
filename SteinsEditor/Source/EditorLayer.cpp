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
					// Steins::Application::Get().Close(); // 애플리케이션 종료 요청
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
		ImGui::Begin("Viewport");
		ImVec2 ImGuiViewportSize = ImGui::GetContentRegionAvail();
		if (viewportSize != Vector2(ImGuiViewportSize.x, ImGuiViewportSize.y))
		{

		}
		// 여기에 렌더링 엔진의 최종 이미지를 ImGui::Image() 로 그릴 것입니다.
		// (지금 당장은 빈 창일 수 있습니다.)
		ImGui::Text("Rendered Scene will go here!");
		ImGui::End();
		ImGui::PopStyleVar();


		// 예시: 속성 패널
		ImGui::Begin("Properties");
		ImGui::Text("Selected Object Properties");
		// 나중에 선택된 객체의 트랜스폼, 머티리얼 속성 등을 표시
		ImGui::End();

		// 예시: 씬 계층 구조 패널
		ImGui::Begin("Scene Hierarchy");
		ImGui::Text("Scene Graph/Hierarchy");
		// 나중에 씬 내의 객체들을 트리 형태로 표시
		ImGui::End();

		// 예시: 자산 브라우저 패널
		ImGui::Begin("Asset Browser");
		ImGui::Text("Project Assets");
		// 나중에 파일 시스템 탐색 및 자산 미리보기
		ImGui::End();

		// 예시: 콘솔/로그 패널
		ImGui::Begin("Console");
		ImGui::Text("Engine Logs and Messages");
		// 엔진의 로그 메시지를 출력
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
