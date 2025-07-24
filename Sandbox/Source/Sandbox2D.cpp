#include "Sandbox2D.h"

#include "imgui/imgui.h"

static Steins::Renderer2D r2d;

Sandbox2D::Sandbox2D()
{
	float squareVertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 0.5f,	0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	};

	//float squareVertices2[4 * 9] = {
	//	-0.3f, -0.4f, 0.0f, 1.0f,0.0f,0.0f,1.0f, 0.0f,1.0f,
	//	 -0.3f,  0.6f, 0.0f,0.0f,0.0f,1.0f,1.0f, 0.0f,0.0f,
	//	 0.7f, 0.6f, 0.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 0.0f,
	//	 0.7f, -0.4f, 0.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 1.0f,
	//};

	Steins::BufferLayout layout =
	{
		{ Steins::ShaderDataType::Float3, "a_Position", "POSITION"},
		{ Steins::ShaderDataType::Float4, "a_Color", "COLOR"},
		{ Steins::ShaderDataType::Float2, "a_TexCoord", "TEXCOORD"}
	};
	//squareVB = Steins::VertexBuffer::CreateStatic(squareVertices, sizeof(squareVertices), layout.GetStride());
	squareVB = Steins::VertexBuffer::CreateDynamic(sizeof(squareVertices), layout.GetStride());
	squareVB->SetData(squareVertices, sizeof(squareVertices));

	//squareVB2 = Steins::VertexBuffer::CreateDynamic(sizeof(squareVertices2), layout.GetStride());
	//squareVB2->SetData(squareVertices2, sizeof(squareVertices2));

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0};
	squareIB = Steins::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

	//uint32_t squareIndices2[6] = { 0, 1, 2, 2, 3, 0 };
	//squareIB2 = Steins::IndexBuffer::Create(squareIndices2, sizeof(squareIndices2) / sizeof(uint32_t));

	
	vs = Steins::Shader::Create("Asset/Shader/VertexShader.hlsl", Steins::ShaderType::Vertex, Steins::ShaderLoadMode::File);
	ps = Steins::Shader::Create("Asset/Shader/PixelShader.hlsl", Steins::ShaderType::Pixel, Steins::ShaderLoadMode::File);
	

	Steins::BufferLayout inputlayout = {
	{ Steins::ShaderDataType::Float3, "a_Position", "POSITION"},
	{ Steins::ShaderDataType::Float4, "a_Color", "COLOR"},
	{ Steins::ShaderDataType::Float2, "a_TexCoord", "TEXCOORD"}
	};

	camera.SetPosition({ 0.0f,0.0f,-1.0f });
	cameraPos = camera.GetViewProjectionMatrix();
	viewProjMat = Steins::ConstantBuffer::Create(sizeof(Steins::Matrix4x4));
	viewProjMat->Update(&cameraPos.mat, sizeof(Steins::Matrix4x4));

	auto path = Steins::FilePath("F:/SteinsReboot/Sandbox/Asset/Texture/Checkerboard.png");
	Steins::TextureDesc textureDesc;
	texture = Steins::Texture2D::Create(path, textureDesc);

	Steins::PipelineStateDesc desc;
	desc.vertexShader = vs;
	desc.pixelShader = ps;
	desc.inputLayout = inputlayout;

	pso = Steins::PipelineState::Create(desc);

	material = Steins::Material::Create(pso);
}

void Sandbox2D::OnUpdate(Float32 _deltaTime)
{
	Steins::RenderCommand::SetClearColor(Steins::Color::White);
	Steins::RenderCommand::Clear();

	if (Steins::Input::GetKeyPress(Steins::Key::Q))
	{
		camera.SetPosition(camera.GetPosition() + Steins::Vector3(0.0f, 0.0f, 1.0f) * _deltaTime);
		cameraPos = camera.GetViewProjectionMatrix();
		viewProjMat->Update(&cameraPos.mat, sizeof(Steins::Matrix4x4));
	}
	if (Steins::Input::GetKeyPress(Steins::Key::E))
	{
		camera.SetPosition(camera.GetPosition() + Steins::Vector3(0.0f, 0.0f, -1.0f) * _deltaTime);
		cameraPos = camera.GetViewProjectionMatrix();
		viewProjMat->Update(&cameraPos.mat, sizeof(Steins::Matrix4x4));
	}

	if (Steins::Input::GetKeyPress(Steins::Key::W))
	{
		camera.SetPosition(camera.GetPosition() + Steins::Vector3(0.0f, 1.0f, 0.0f) * _deltaTime);
		cameraPos = camera.GetViewProjectionMatrix();
		viewProjMat->Update(&cameraPos.mat, sizeof(Steins::Matrix4x4));
	}

	if (Steins::Input::GetKeyPress(Steins::Key::A))
	{
		camera.SetPosition(camera.GetPosition() + Steins::Vector3(-1.0f, 0.0f, 0.0f) * _deltaTime);
		cameraPos = camera.GetViewProjectionMatrix();
		viewProjMat->Update(&cameraPos.mat, sizeof(Steins::Matrix4x4));
	}

	if (Steins::Input::GetKeyPress(Steins::Key::S))
	{
		camera.SetPosition(camera.GetPosition() + Steins::Vector3(0.0f, -1.0f, 0.0f) * _deltaTime);
		cameraPos = camera.GetViewProjectionMatrix();
		viewProjMat->Update(&cameraPos.mat, sizeof(Steins::Matrix4x4));
	}

	if (Steins::Input::GetKeyPress(Steins::Key::D))
	{
		camera.SetPosition(camera.GetPosition() + Steins::Vector3(1.0f, 0.0f, 0.0f) * _deltaTime);
		cameraPos = camera.GetViewProjectionMatrix();
		viewProjMat->Update(&cameraPos.mat, sizeof(Steins::Matrix4x4));
	}

	////camera.SetPosition({ 0.5f, 0.5f, 0.0f });

	//Steins::Renderer::BeginScene(camera);
	pso->Bind();
	squareVB->Bind();
	squareIB->Bind();
	material->Bind();
	material->SetTexture2D("u_Texture", texture);
	material->SetConstantBuffer("Camera", viewProjMat);
	
	//viewProjMat->Bind(0, Steins::SteinsVertexBit);
	//texture->Bind(0);
	//Steins::RenderCommand::DrawIndexed(squareIB->GetCount());
	Steins::Renderer::Submit(squareIB->GetCount());

	//r2d.TestTick();
	//Steins::Renderer2D::BeginScene(camera);
	//Steins::Renderer2D::DrawQuad(Steins::Vector3(0.2f, 0.2f, 0.0f), Steins::Vector2(1.0f, 1.0f), { 1.0f,1.0f,1.0f,1.0f });
	//Steins::Renderer2D::DrawQuad(Steins::Vector3(-0.2f, -0.2f, 0.0f), Steins::Vector2(1.0f, 1.0f), { 1.0f,1.0f,1.0f,1.0f });
	//Steins::Renderer2D::EndScene();
	//va->Bind();
	//vs->Bind();
	//ps->Bind();
	////Steins::Matrix4x4 transform = Steins::Math::Translate(Steins::Matrix4x4(), Steins::Vector4(1.0f, 1.0f, 0.0f));
	////transform.Transpose();


}

void Sandbox2D::OnImGuiRender()
{
	ImGui::ShowDemoWindow();

	ImGui::Begin("ImGui texture Image Test");
	//ImGui::Image((ImTextureID)(uintptr_t)texture->GetNativeHandle(), ImVec2(texture->GetWidth(), texture->GetHeight()));
	//ImGui::Image((ImTextureID)(ID3D11ShaderResourceView*)texture->GetNativeHandle(), ImVec2(texture->GetWidth(), texture->GetHeight()));
	//ImGui::Image((ImTextureID)static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(texture->GetNativeHandle())), ImVec2(texture->GetWidth() * 2, texture->GetHeight() * 2));
	//ImGui::Image((ImTextureID)(texture->GetNativeHandle()), ImVec2(texture->GetWidth() * 2, texture->GetHeight() * 2));
	ImGui::End();
}

