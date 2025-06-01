#include <Steins.h>
#include <Steins/Core/EntryPoint.h>

#include "EditorLayer.h"

class SteinsEditorApp: public Steins::Application
{
public:
	SteinsEditorApp(const Steins::ApplicationSpecification& _spec)
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
	spec.rendererAPI = RendererAPIType::DirectX11;

	return new SteinsEditorApp(spec);
}