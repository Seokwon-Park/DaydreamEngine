#include <Daydream.h>
#include "Daydream/Core/EntryPoint.h"

#include "imgui/imgui.h"

#include "Sandbox2D.h"

class Sandbox : public Daydream::Application
{
public:
	Sandbox(const Daydream::ApplicationSpecification& _specification)
		:Daydream::Application(_specification)
	{
		//AttachLayer(new ExampleLayer());
		AttachOverlay(new Daydream::Sandbox2D());
	}

	virtual ~Sandbox() override
	{

	}
};

Daydream::Application* Daydream::CreateApplication()
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Lab";
	spec.rendererAPI = RendererAPIType::DirectX11;

	return new Sandbox(spec); 
}