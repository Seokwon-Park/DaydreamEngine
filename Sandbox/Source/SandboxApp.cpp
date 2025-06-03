#include <Steins.h>
#include "Steins/Core/EntryPoint.h"

#include "imgui/imgui.h"

#include "Sandbox2D.h"

class ExampleLayer : public Steins::Layer
{
public:
	ExampleLayer()
		:Layer("Test"), camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{

		//으이구 멍청아.
		//dx11 default(cw), gl default(ccw)

		//float vertices[4 * 7] = {
		//	-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,1.0f,
		//	 0.0f,  0.5f, 0.0f,0.0f,0.0f,1.0f,1.0f,
		//	 0.5f, -0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f,
		//	 1.0f, 0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f,
		//};
		float vertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,1.0f, 0.0f,1.0f,
			 0.0f,  0.5f, 0.0f,0.0f,0.0f,1.0f,1.0f, 0.0f,0.0f,
			 0.5f, -0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 1.0f,
			 1.0f, 0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 0.0f
		};
		// 
		// 
//		float vertices[3 * 3] = {
//			-0.5f, -0.5f, 0.0f,
//			 0.5f, -0.5f, 0.0f,
//			 0.0f,  0.5f, 0.0f
//		};
////

		Steins::BufferLayout layout = 
		{
			{ Steins::ShaderDataType::Float3, "a_Position", "POSITION"},
			{ Steins::ShaderDataType::Float4, "a_Color", "COLOR"},
			{ Steins::ShaderDataType::Float2, "a_TexCoord", "TEXCOORD"}
		};
		vb = Steins::VertexBuffer::Create(vertices, sizeof(vertices),layout.GetStride());

		vb->SetLayout(layout);
		//
		unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };
		ib = Steins::IndexBuffer::Create(indices, 6);

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
			vs = Steins::Shader::Create("Asset/Shader/VertexShader.hlsl", Steins::ShaderType::Vertex, Steins::ShaderLoadMode::File);
			ps = Steins::Shader::Create("Asset/Shader/PixelShader.hlsl", Steins::ShaderType::Pixel, Steins::ShaderLoadMode::File);
		}
		if (Steins::Renderer::GetAPI() == Steins::RendererAPIType::OpenGL)
		{

			std::string vertexSrc = R"(
			#version 450 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec2 a_TexCoord;
			
			layout(row_major, set = 0, std140, binding = 0) uniform Camera
			{
			    mat4 u_ViewProjection;
			};
			
			out gl_PerVertex 
			{
				vec4 gl_Position;
			};

			layout(location = 0) out vec3 v_Position;
			layout(location = 1) out vec4 v_Color;
			layout(location = 2) out vec2 v_TexCoord;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * vec4(v_Position, 1.0f);
			}
		)";
			std::string pixelSrc = R"(
			#version 450 core
			
			layout(location = 0) out vec4 color;

			layout(location = 0) in vec3 v_Position;
			layout(location = 1) in vec4 v_Color;
			layout(location = 2) in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				//color = vec4(v_Position * 0.5 + 0.5, 1.0);
				//color = vec4(v_TexCoord.x, v_TexCoord.y, 0.0f, 1.0f);
				color = texture(u_Texture, v_TexCoord);
			}
		)";

			vs = Steins::Shader::Create(vertexSrc, Steins::ShaderType::Vertex, Steins::ShaderLoadMode::Source);
			ps = Steins::Shader::Create(pixelSrc, Steins::ShaderType::Pixel, Steins::ShaderLoadMode::Source);
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

		camera.SetPosition({ 0.3f,0.0f,0.0f });
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

	void OnUpdate(Float32 _deltaTime) override
	{
		Steins::RenderCommand::SetClearColor(Steins::Color::White);
		Steins::RenderCommand::Clear();

		////camera.SetPosition({ 0.5f, 0.5f, 0.0f });

		//Steins::Renderer::BeginScene(camera);
		vb->Bind();
		ib->Bind();
		pso->Bind();
		viewProjMat->Bind(0, Steins::SteinsVertexBit);
		texture->Bind(0);


		//va->Bind();
		//vs->Bind();
		//ps->Bind();
		////Steins::Matrix4x4 transform = Steins::Math::Translate(Steins::Matrix4x4(), Steins::Vector4(1.0f, 1.0f, 0.0f));
		////transform.Transpose();

		Steins::Renderer::Submit(ib->GetCount());

		//STEINS_INFO("TestLayer::UPDATE");
		/*if(Steins::Input::GetKeyDown(Steins::Key::Up))
		{
			STEINS_INFO("GOOD");
		}*/
	}

	void OnEvent(Steins::Event& _event) override
	{
		//STEINS_TRACE("{0}", _event);
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("OpenGL Texture Text");
		//ImGui::Image((ImTextureID)(uintptr_t)texture->GetNativeHandle(), ImVec2(texture->GetWidth(), texture->GetHeight()));
		//ImGui::Image((ImTextureID)(ID3D11ShaderResourceView*)texture->GetNativeHandle(), ImVec2(texture->GetWidth(), texture->GetHeight()));
		//ImGui::Image((ImTextureID)static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(texture->GetNativeHandle())), ImVec2(texture->GetWidth() * 2, texture->GetHeight() * 2));
		ImGui::Image((ImTextureID)(texture->GetNativeHandle()), ImVec2(texture->GetWidth() * 2, texture->GetHeight() * 2));	
		ImGui::End();
	}
private:
	Steins::Shared<Steins::VertexBuffer> vb;
	Steins::Shared<Steins::IndexBuffer> ib;
	Steins::Shared<Steins::Shader> vs;
	Steins::Shared<Steins::Shader> ps;
	Steins::Shared<Steins::PipelineState> pso;
	Steins::Shared<Steins::ConstantBuffer> viewProjMat;
	Steins::Shared<Steins::Texture2D> texture;

	Steins::OrthographicCamera camera;
	Steins::Matrix4x4 cameraPos;
};

class Sandbox : public Steins::Application
{
public:
	Sandbox(const Steins::ApplicationSpecification& _specification)
		:Steins::Application(_specification)
	{
		//AttachLayer(new ExampleLayer());
		AttachOverlay(new Sandbox2D());
	}

	virtual ~Sandbox() override
	{

	}
};

Steins::Application* Steins::CreateApplication()
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Lab";
	spec.rendererAPI = RendererAPIType::Vulkan;

	return new Sandbox(spec);
}