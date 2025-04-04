#include "SteinsPCH.h"
#include "ImGuiHelper.h"
#include "Steins/Core/Application.h"

#include "Steins/Render/Renderer.h"

#include "GLFW/glfw3.h"

namespace Steins
{
	void ImGuiHelper::Init()
	{
		Application& app = Application::GetInstance();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetMainWindow().GetNativeWindow());
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::OpenGL:
		{
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 410");
			break;
		}
		case RendererAPIType::DirectX11:
		{
			ImGui_ImplGlfw_InitForOther(window, true);
			ImGui_ImplDX11_Init();
			break;
		}
		case RendererAPIType::DirectX12:
		{
			ImGui_ImplGlfw_InitForOther(window, true);
			ImGui_ImplDX12_Init();
				break;
		}
		case RendererAPIType::Vulkan:
		{
			ImGui_ImplGlfw_InitForVulkan(window, true);
			ImGui_ImplVulkan_Init();
		}

		default:
			break;
		} 
	}
	void ImGuiHelper::Shutdown()
	{
	}
	void ImGuiHelper::NewFrame()
	{
	}
	void ImGuiHelper::RenderRawData()
	{
	}
}