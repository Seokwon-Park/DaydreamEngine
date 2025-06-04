#include "EditorLayer.h"
#include "imgui/imgui.h"

EditorLayer::EditorLayer()
	:Layer("Editor Layer")
{
	float squareVertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,1.0f, 0.0f,1.0f,
			 -0.5f,  0.5f, 0.0f,0.0f,0.0f,1.0f,1.0f, 0.0f,0.0f,
			 0.5f, 0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 1.0f
	};

	Steins::BufferLayout layout =
	{
		{ Steins::ShaderDataType::Float3, "a_Position", "POSITION"},
		{ Steins::ShaderDataType::Float4, "a_Color", "COLOR"},
		{ Steins::ShaderDataType::Float2, "a_TexCoord", "TEXCOORD"}
	};
	squareVB = Steins::VertexBuffer::Create(squareVertices, sizeof(squareVertices), layout.GetStride());

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	squareIB = Steins::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

	if (Steins::Renderer::GetAPI() == Steins::RendererAPIType::DirectX11 ||
		Steins::Renderer::GetAPI() == Steins::RendererAPIType::DirectX12)
	{
		vs = Steins::Shader::Create("Asset/Shader/VertexShader.hlsl", Steins::ShaderType::Vertex, Steins::ShaderLoadMode::File);
		ps = Steins::Shader::Create("Asset/Shader/PixelShader.hlsl", Steins::ShaderType::Pixel, Steins::ShaderLoadMode::File);
	}
	if (Steins::Renderer::GetAPI() == Steins::RendererAPIType::OpenGL)
	{
		vs = Steins::Shader::Create("Asset/Shader/Quad.vert", Steins::ShaderType::Vertex, Steins::ShaderLoadMode::File);
		ps = Steins::Shader::Create("Asset/Shader/Quad.frag", Steins::ShaderType::Pixel, Steins::ShaderLoadMode::File);
	}
	if (Steins::Renderer::GetAPI() == Steins::RendererAPIType::Vulkan)
	{
		vs = Steins::Shader::Create("Asset/Shader/QuadTexVS.spv", Steins::ShaderType::Vertex, Steins::ShaderLoadMode::File);
		ps = Steins::Shader::Create("Asset/Shader/QuadTexPS.spv", Steins::ShaderType::Pixel, Steins::ShaderLoadMode::File);
	}


	Steins::BufferLayout inputlayout = {
	{ Steins::ShaderDataType::Float3, "a_Position", "POSITION"},
	{ Steins::ShaderDataType::Float4, "a_Color", "COLOR"},
	{ Steins::ShaderDataType::Float2, "a_TexCoord", "TEXCOORD"}

	};

	camera.SetPosition({ 0.0f,0.0f,0.0f });
	cameraPos = camera.GetViewProjectionMatrix();
	viewProjMat = Steins::ConstantBuffer::Create(sizeof(Steins::Matrix4x4));
	viewProjMat->Update(&cameraPos.mat, sizeof(Steins::Matrix4x4));

	auto path = Steins::FilePath("Asset/Texture/Checkerboard.png");
	texture = Steins::Texture2D::Create(path);

	Steins::PipelineStateDesc desc;
	desc.vertexShader = vs;
	desc.pixelShader = ps;
	desc.inputLayout = inputlayout;
	desc.constantBuffers = { { 0,viewProjMat} };
	desc.textures = { texture };

	pso = Steins::PipelineState::Create(desc);
}

void EditorLayer::OnUpdate(Float32 _deltaTime)
{
	Steins::RenderCommand::SetClearColor(Steins::Color::White);
	Steins::RenderCommand::Clear();

	////camera.SetPosition({ 0.5f, 0.5f, 0.0f });

	squareVB->Bind();
	squareIB->Bind();
	pso->Bind();
	viewProjMat->Bind(0, Steins::SteinsVertexBit);
	texture->Bind(0);
	Steins::Renderer::Submit(squareIB->GetCount());
	//Steins::Renderer::BeginScene(camera);
	//squareVB->Bind();
	//squareIB->Bind();
	//pso->Bind();
	//viewProjMat->Bind(0, Steins::SteinsVertexBit);
	//texture->Bind(0);
	//Steins::Renderer::Submit(squareIB->GetCount());
	Steins::OrthographicCamera camera = Steins::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
	Steins::Renderer2D::BeginScene(camera);
	Steins::Renderer2D::DrawQuad(Steins::Vector3(0.2f, 0.2f, 0.0f), Steins::Vector2(1.0f, 1.0f), { 1.0f,1.0f,1.0f,1.0f });
	Steins::Renderer2D::DrawQuad(Steins::Vector3(-0.2f, -0.2f, 0.0f), Steins::Vector2(1.0f, 1.0f), { 1.0f,1.0f,1.0f,1.0f });
	Steins::Renderer2D::EndScene();
	//va->Bind();
	//vs->Bind();
	//ps->Bind();
	////Steins::Matrix4x4 transform = Steins::Math::Translate(Steins::Matrix4x4(), Steins::Vector4(1.0f, 1.0f, 0.0f));
	////transform.Transpose();


}

void EditorLayer::OnImGuiRender()
{
	static bool dockspaceOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	//ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Steins::Application::GetInstance().isMaximzed ? ImVec2(6.0f, 6.0f) : ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, false ? ImVec2(6.0f, 6.0f) : ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });

	ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
	//m_DockSpacePos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
	//STS_CORE_WARN("Dock Space Coord = {0}, {1}", m_DockSpacePos.x, m_DockSpacePos.y);
	ImGui::PopStyleColor(); // MenuBarBg
	ImGui::PopStyleVar(2);

	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();

	ImGui::ShowDemoWindow();
}
