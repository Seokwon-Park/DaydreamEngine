#include "SteinsPCH.h"
#include "ImGuiHelper.h"
#include "Steins/Core/Application.h"

namespace Steins
{
	void ImGuiHelper::InitImGuiLayer(SteinsWindow* _window)
	{
		switch (Application::GetRendererAPI())
		{
		case RendererAPI::OpenGL:

		case RendererAPI::DirectX11:

		default:
			break;
		} 
	}
}