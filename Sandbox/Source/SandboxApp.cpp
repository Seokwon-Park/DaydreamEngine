#include <Steins.h>

class ExampleLayer : public Steins::Layer 
{
public :
	ExampleLayer()
		:Layer("Test"), camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{

		//으이구 멍청아.
		//dx11 default(cw), gl default(ccw)
		float vertices[3 * 7] = {
					 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
					 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
					-0.5f, -0.5f, 0.0f, 1.0f, 0.0, 0.0f, 1.0f,
		};
//
		va = Steins::VertexArray::Create();

		vb = Steins::VertexBuffer::Create(vertices, sizeof(vertices));
		Steins::BufferLayout layout = {
			{ Steins::ShaderDataType::Float3, "a_Position", "POSITION"},
			{ Steins::ShaderDataType::Float4, "a_Color", "COLOR"}
		};
		vb->SetLayout(layout);
		va->AddVertexBuffer(vb);

		unsigned int indices[3] = { 0, 1, 2 };
		ib = Steins::IndexBuffer::Create(indices, 3);
		va->SetIndexBuffer(ib);

//		std::string vertexSrc = R"(
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
//		std::string pixelSrc = R"(
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



		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";
		std::string pixelSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		vs = Steins::Shader::Create(vertexSrc, Steins::ShaderType::Vertex);
		ps = Steins::Shader::Create(pixelSrc, Steins::ShaderType::Pixel);
		//vs->SetVertexArray(va);
	}

	void OnUpdate() override
	{
		Steins::RenderCommand::SetClearColor(Steins::Color::White);
		Steins::RenderCommand::Clear();

		camera.SetPosition({ 0.5f, 0.5f, 0.0f });

		Steins::Renderer::BeginScene(camera);

		va->Bind();
		vs->Bind();
		ps->Bind();
		//Steins::Matrix4x4 transform = Steins::Math::Translate(Steins::Matrix4x4(), Steins::Vector4(1.0f, 1.0f, 0.0f));
		//transform.Transpose();

		Steins::Renderer::Submit(va);

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
private:
	Steins::Shared<Steins::VertexArray> va;
	Steins::Shared<Steins::VertexBuffer> vb;
	Steins::Shared<Steins::IndexBuffer> ib;
	Steins::Shared<Steins::Shader> vs;
	Steins::Shared<Steins::Shader> ps;

	Steins::OrthographicCamera camera;
};

class Sandbox : public Steins::Application
{
public:
	Sandbox(const Steins::ApplicationSpecification& _specification)
		:Steins::Application(_specification)
	{
		AttachLayer(new ExampleLayer());
		//AttachOverlay(new Steins::ImGuiLayer());
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
	spec.rendererAPI = RendererAPIType::OpenGL;

	return new Sandbox(spec);
}