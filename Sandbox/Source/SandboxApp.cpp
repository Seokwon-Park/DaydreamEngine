#include <Steins.h>
#include "Steins/Core/EntryPoint.h"

#include "imgui/imgui.h"

#include "Sandbox2D.h"

class Sandbox : public Steins::Application
{
public:
	Sandbox(const Steins::ApplicationSpecification& _specification)
		:Steins::Application(_specification)
	{
		//AttachLayer(new ExampleLayer());
		AttachOverlay(new Steins::Sandbox2D());
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