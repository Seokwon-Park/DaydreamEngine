#include "Sandbox2D.h"

#include "imgui/imgui.h"

static Steins::Renderer2D r2d;

Sandbox2D::Sandbox2D()
{
	float squareVertices[8 * 9] = {
			-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,1.0f, 0.0f,1.0f,
			 -0.5f,  0.5f, 0.0f,0.0f,0.0f,1.0f,1.0f, 0.0f,0.0f,
			 0.5f, 0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 1.0f,
	};

	float squareVertices2[4 * 9] = {
		-0.3f, -0.4f, 0.0f, 1.0f,0.0f,0.0f,1.0f, 0.0f,1.0f,
		 -0.3f,  0.6f, 0.0f,0.0f,0.0f,1.0f,1.0f, 0.0f,0.0f,
		 0.7f, 0.6f, 0.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 0.0f,
		 0.7f, -0.4f, 0.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 1.0f,
	};

	Steins::BufferLayout layout =
	{
		{ Steins::ShaderDataType::Float3, "a_Position", "POSITION"},
		{ Steins::ShaderDataType::Float4, "a_Color", "COLOR"},
		{ Steins::ShaderDataType::Float2, "a_TexCoord", "TEXCOORD"}
	};
	//squareVB = Steins::VertexBuffer::CreateStatic(squareVertices, sizeof(squareVertices), layout.GetStride());
	squareVB = Steins::VertexBuffer::CreateDynamic(sizeof(squareVertices), layout.GetStride());
	squareVB->SetData(squareVertices, 4*4*9);

	squareVB2 = Steins::VertexBuffer::CreateDynamic(sizeof(squareVertices2), layout.GetStride());
	squareVB2->SetData(squareVertices2, sizeof(squareVertices2));

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0};
	squareIB = Steins::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

	uint32_t squareIndices2[6] = { 0, 1, 2, 2, 3, 0 };
	squareIB2 = Steins::IndexBuffer::Create(squareIndices2, sizeof(squareIndices2) / sizeof(uint32_t));

	if (Steins::Renderer::GetAPI() == Steins::RendererAPIType::DirectX11 ||
		Steins::Renderer::GetAPI() == Steins::RendererAPIType::DirectX12)
	{
		//			std::string vertexSrc = R"(
		//struct VSInput
		//{
		//    float3 position : POSITION;
		//    float4 color    : COLOR0;
		//};
		//
		//struct VSOutput
		//{
		//    float4 position : SV_Position;
		//    float4 color    : COLOR0;
		//};
		//
		//VSOutput VSMain(VSInput input)
		//{
		//    VSOutput output = (VSOutput)0;
		//    output.position = float4(input.position, 1.0); // 월드 변환 생략
		//    output.color = input.color;
		//    return output;
		//}
		//		)";
		//
		//			std::string pixelSrc = R"(
		//struct PSInput
		//{
		//    float4 position: SV_Position;
		//    float4 color: COLOR0;
		//};
		//
		//struct PSOutput
		//{
		//    float4 color: SV_Target0;
		//};
		//
		//PSOutput PSMain(PSInput input)
		//{
		//    PSOutput output = (PSOutput)0;
		//    output.color = input.color;
		//    return output;
		//}
		//		)";
		vs = Steins::Shader::Create("F:/SteinsReboot/Sandbox/Asset/Shader/VertexShader.hlsl", Steins::ShaderType::Vertex, Steins::ShaderLoadMode::File);
		ps = Steins::Shader::Create("F:/SteinsReboot/Sandbox/Asset/Shader/PixelShader.hlsl", Steins::ShaderType::Pixel, Steins::ShaderLoadMode::File);
	}
	if (Steins::Renderer::GetAPI() == Steins::RendererAPIType::OpenGL)
	{

		//std::string vertexSrc = R"(
		//	#version 450 core
		//	
		//	layout(location = 0) in vec3 a_Position;
		//	layout(location = 1) in vec4 a_Color;
		//	layout(location = 2) in vec2 a_TexCoord;
		//	
		//	layout(row_major, set = 0, std140, binding = 0) uniform Camera
		//	{
		//	    mat4 u_ViewProjection;
		//	};
		//	
		//	out gl_PerVertex 
		//	{
		//		vec4 gl_Position;
		//	};
		//
		//	layout(location = 0) out vec3 v_Position;
		//	layout(location = 1) out vec4 v_Color;
		//	layout(location = 2) out vec2 v_TexCoord;
		//
		//	void main()
		//	{
		//		v_Position = a_Position;
		//		v_Color = a_Color;
		//		v_TexCoord = a_TexCoord;
		//		gl_Position = u_ViewProjection * vec4(v_Position, 1.0f);
		//	}
		//)";
		//std::string pixelSrc = R"(
		//	#version 450 core
		//	
		//	layout(location = 0) out vec4 color;
		//
		//	layout(location = 0) in vec3 v_Position;
		//	layout(location = 1) in vec4 v_Color;
		//	layout(location = 2) in vec2 v_TexCoord;
		//
		//	uniform sampler2D u_Texture;
		//
		//	void main()
		//	{
		//		//color = vec4(v_Position * 0.5 + 0.5, 1.0);
		//		//color = vec4(v_TexCoord.x, v_TexCoord.y, 0.0f, 1.0f);
		//		color = texture(u_Texture, v_TexCoord);
		//	}
		//)";

		vs = Steins::Shader::Create("Asset/Shader/Quad.vert", Steins::ShaderType::Vertex, Steins::ShaderLoadMode::File);
		ps = Steins::Shader::Create("Asset/Shader/Quad.frag", Steins::ShaderType::Pixel, Steins::ShaderLoadMode::File);
	}
	if (Steins::Renderer::GetAPI() == Steins::RendererAPIType::Vulkan)
	{
		//vs = Steins::Shader::Create("Asset/Shader/QuadVS.spv", Steins::ShaderType::Vertex, Steins::ShaderLoadMode::File);
		//ps = Steins::Shader::Create("Asset/Shader/QuadPS.spv", Steins::ShaderType::Pixel, Steins::ShaderLoadMode::File);
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

	auto path = Steins::FilePath("F:/SteinsReboot/Sandbox/Asset/Texture/Checkerboard.png");
	texture = Steins::Texture2D::Create(path);

	Steins::PipelineStateDesc desc;
	desc.vertexShader = vs;
	desc.pixelShader = ps;
	desc.inputLayout = inputlayout;
	desc.constantBuffers = { { 0,viewProjMat} };
	desc.textures = { texture };

	pso = Steins::PipelineState::Create(desc);

	//r2d.Test();
}

void Sandbox2D::OnUpdate(Float32 _deltaTime)
{
	Steins::RenderCommand::SetClearColor(Steins::Color::White);
	Steins::RenderCommand::Clear();

	////camera.SetPosition({ 0.5f, 0.5f, 0.0f });

	//Steins::Renderer::BeginScene(camera);
	squareVB->Bind();
	squareIB->Bind();
	pso->Bind();
	viewProjMat->Bind(0, Steins::SteinsVertexBit);
	texture->Bind(0);
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

	//squareVB2->Bind();
	//squareIB2->Bind();
	//Steins::Renderer::Submit(squareIB2->GetCount());
}

