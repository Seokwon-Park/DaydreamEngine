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
	}

	void OnUpdate(Float32 _deltaTime) override
	{
	}

	void OnEvent(Steins::Event& _event) override
	{
		//STEINS_TRACE("{0}", _event);
	}

	void OnImGuiRender() override
	{

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
	spec.rendererAPI = RendererAPIType::DirectX11;

	return new Sandbox(spec);
}