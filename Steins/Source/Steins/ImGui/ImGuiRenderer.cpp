#include "SteinsPCH.h"
#include "ImGuiRenderer.h"
#include "Steins/Core/Application.h"

#include "Steins/Render/Renderer.h"
#include "backends/imgui_impl_glfw.h"

#include "GLFW/glfw3.h"

namespace Steins
{
	void ImGuiRenderer::Init(SteinsWindow* _window)
	{
		if (mainWindow != nullptr)
		{
			STEINS_CORE_WARN("You Already Initialized Imgui Renderer!");
			return;
		}
		mainWindow = _window;
		GLFWwindow* window = (GLFWwindow*)_window->GetNativeWindow();
		switch (Renderer::GetRenderDevice()->GetAPI())
		{
		case RendererAPIType::None:
			STEINS_CORE_ERROR("None API is not supported");
			break;
		case RendererAPIType::OpenGL:
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			break;
		case RendererAPIType::DirectX11:
			ImGui_ImplGlfw_InitForOther(window, true);
			break;
		case RendererAPIType::DirectX12:
			ImGui_ImplGlfw_InitForOther(window, true);
			break;
		case RendererAPIType::Vulkan:
			ImGui_ImplGlfw_InitForVulkan(window, true);
			break;
		case RendererAPIType::Metal:
			ImGui_ImplGlfw_InitForOther(window, true);
			break;
		default:
			break;
		}
	}

	void ImGuiRenderer::Shutdown()
	{
		ImGui_ImplGlfw_Shutdown();

	}

	void ImGuiRenderer::NewFrame()
	{
		ImGui_ImplGlfw_NewFrame();
	}

	void ImGuiRenderer::Render()
	{
		ImGui::Render();
		//mainWindow->GetSwapChain()->GetBackFramebuffer()->Bind();
	}		
}