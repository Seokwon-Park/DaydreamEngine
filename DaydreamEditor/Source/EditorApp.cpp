#include <Daydream.h>
#include <Daydream/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Daydream
{
	class EditorApplication : public Application
	{
	public:
		EditorApplication(const Daydream::ApplicationSpecification& _spec)
			:Daydream::Application(_spec)
		{
			AttachLayer(new EditorLayer());
		}
	};

	Application* CreateApplication()
	{
		ApplicationSpecification spec;
		spec.Name = "Daydream Engine Editor";
		spec.WorkingDirectory = "../Lab";
		spec.rendererAPI = RendererAPIType::DirectX11;

		return new EditorApplication(spec);
	}
}