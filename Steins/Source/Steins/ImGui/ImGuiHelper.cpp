#include "SteinsPCH.h"
#include "ImGuiHelper.h"
#include "Steins/Core/Application.h"

#include "Steins/Render/Renderer.h"
#include "Platform/RenderSystem/OpenGL/OpenGLGraphicsDevice.h"
#include "Platform/RenderSystem/DirectX11/D3D11GraphicsDevice.h"

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
			OpenGLGraphicsDevice* device = app.GetMainWindow().GetNativeDevice<OpenGLGraphicsDevice>();
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init(device->GetVersion().data());
			break;
		}
		case RendererAPIType::DirectX11:
		{
			D3D11GraphicsDevice* device = app.GetMainWindow().GetNativeDevice<D3D11GraphicsDevice>();
			ImGui_ImplGlfw_InitForOther(window, true);
			ImGui_ImplDX11_Init(device->GetDevice(), device->GetContext());
			break;
		}
		//case RendererAPIType::DirectX12:
		//{
		//	ImGui_ImplGlfw_InitForOther(window, true);
		//	ImGui_ImplDX12_Init();
		//	break;
		//}
		//case RendererAPIType::Vulkan:
		//{
		//	ImGui_ImplGlfw_InitForVulkan(window, true);
		//	ImGui_ImplVulkan_Init();
		//}
		default:
			break;
		} 
	}
	void ImGuiHelper::Shutdown()
	{
	}
	void ImGuiHelper::NewFrame()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::OpenGL:
		{
			ImGui_ImplOpenGL3_NewFrame();
			break;
		}
		case RendererAPIType::DirectX11:
		{
			ImGui_ImplDX11_NewFrame();
			break;
		}
		case RendererAPIType::DirectX12:
		{
			ImGui_ImplDX12_NewFrame();
			break;
		}
		case RendererAPIType::Vulkan:
		{
			ImGui_ImplVulkan_NewFrame();
			break;
		}
		//case RendererAPIType::Metal:
		//{
		//	ImGui_ImplMetal_NewFrame();
		//}
		default:
			break;
		}
		ImGui_ImplGlfw_NewFrame();
	}
	void ImGuiHelper::RenderRawData()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::OpenGL:
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			break;
		}
		case RendererAPIType::DirectX11:
		{
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			break;
		}
		case RendererAPIType::DirectX12:
		{
			//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData());
			break;
		}
		case RendererAPIType::Vulkan:
		{
			//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData());
			break;
		}
		case RendererAPIType::Metal:
		{
			//ImGui_ImplMetal_
			break;
		}
		default:
			break;
		}
	}
}