#include <Steins.h>
#include <Steins/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Steins
{
	class EditorApplication : public Application
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
		spec.Name = "Steins Engine Editor";
		spec.WorkingDirectory = "../Lab";
		spec.rendererAPI = RendererAPIType::OpenGL;

		return new EditorApplication(spec);
	}
}