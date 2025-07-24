#include <Steins.h>
#include <Steins/Core/EntryPoint.h>

#include "EditorLayer.h"

class EditorApplication: public Steins::Application
{
public:
	EditorApplication(const Steins::ApplicationSpecification& _spec)
		:Steins::Application(_spec)
	{
		AttachOverlay(new EditorLayer());
	}
};

Steins::Application* Steins::CreateApplication()
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Lab";
	spec.rendererAPI = RendererAPIType::DirectX12;

	return new EditorApplication(spec);
}