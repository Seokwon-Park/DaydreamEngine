#include "SteinsPCH.h"
#include "ImGuiHelper.h"
#include "Steins/Core/Application.h"

#include "Steins/Render/Renderer.h"
#include "Platform/RenderSystem/OpenGL/OpenGLGraphicsDevice.h"
#include "Platform/RenderSystem/Vulkan/VulkanGraphicsDevice.h"
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
			OpenGLGraphicsDevice* device = app.GetGraphicsDevice()->Get<OpenGLGraphicsDevice>();
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init(device->GetVersion().data());
			break;
		}
		case RendererAPIType::DirectX11:
		{
			D3D11GraphicsDevice* device = app.GetGraphicsDevice()->Get<D3D11GraphicsDevice>();
			ImGui_ImplGlfw_InitForOther(window, true);
			ImGui_ImplDX11_Init(device->GetDevice(), device->GetContext());
			break;
		}
		case RendererAPIType::DirectX12:
		{
			D3D12GraphicsDevice* device = app.GetGraphicsDevice()->Get<D3D12GraphicsDevice>();
			ImGui_ImplDX12_InitInfo info{};
			info.Device = device->GetDevice();
			info.CommandQueue = device->GetCommandQueue();     // Command queue used for queuing texture uploads.
			info.NumFramesInFlight = 2;
			info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;          // RenderTarget format.
			info.DSVFormat = DXGI_FORMAT_UNKNOWN;          // DepthStencilView format.
			info.UserData = device;
			info.SrvDescriptorHeap = device->GetSRVHeap();
			info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo* _info, D3D12_CPU_DESCRIPTOR_HANDLE* _outCpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* _outGpuHandle)
				{
					((D3D12GraphicsDevice*)_info->UserData)->GetSRVHeapAlloc().Alloc(_outCpuHandle, _outGpuHandle);
				};
			info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo* _info, D3D12_CPU_DESCRIPTOR_HANDLE _outCpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE _outGpuHandle)
				{
					((D3D12GraphicsDevice*)_info->UserData)->GetSRVHeapAlloc().Free(_outCpuHandle, _outGpuHandle);
				};

			ImGui_ImplGlfw_InitForOther(window, true);
			ImGui_ImplDX12_Init(&info);
			break;
		}
		case RendererAPIType::Vulkan:
		{
			VulkanGraphicsDevice* device = app.GetGraphicsDevice()->Get<VulkanGraphicsDevice>();
			ImGui_ImplVulkan_InitInfo info{};
			//info.ApiVersion =;
			info.Instance = device->GetInstance();
			info.PhysicalDevice = device->GetPhysicalDevice();
			info.Device = device->GetLogicalDevice();
			info.QueueFamily = device->GetQueueFamily();
			info.Queue = device->GetQueue();
			//info.DescriptorPool;
			//info.RenderPass;
			info.MinImageCount = 2;
			//info.ImageCount;
			//info.MSAASamples;

			ImGui_ImplGlfw_InitForVulkan(window, true);
			ImGui_ImplVulkan_Init(&info);
			break;
		}
		default:
			break;
		}
	}
	void ImGuiHelper::Shutdown()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::OpenGL:
		{
			ImGui_ImplOpenGL3_Shutdown();
			break;
		}
		case RendererAPIType::DirectX11:
		{
			ImGui_ImplDX11_Shutdown();
			break;
		}
		case RendererAPIType::DirectX12:
		{
			ImGui_ImplDX12_Shutdown();
			break;
		}
		case RendererAPIType::Vulkan:
		{
			ImGui_ImplVulkan_Shutdown();
			break;
		}
		case RendererAPIType::Metal:
		{
			//ImGui_ImplMetal_Shutdown();
			break;
		}
		default: 
			break;
		}
		ImGui_ImplGlfw_Shutdown();
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

	void ImGuiHelper::Render()
	{
		ImGui::Render();
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::OpenGL:
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			break;
		}
		case RendererAPIType::DirectX11:
		{
			D3D11GraphicsDevice* device = Application::GetGraphicsDevice()->Get<D3D11GraphicsDevice>();
			Application::GetInstance().GetMainWindow().GetSwapchain()->GetBackFramebuffer()->Bind();
			//auto rtvs = device->
			//device->GetContext()->OMSetRenderTargets(1, , nullptr);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			break;
		}
		case RendererAPIType::DirectX12:
		{
			Application& app = Application::GetInstance();
			D3D12GraphicsDevice* device = app.GetGraphicsDevice()->Get<D3D12GraphicsDevice>();
			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), device->GetCommandList());
			break;
		}
		case RendererAPIType::Vulkan:
		{
			//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),);
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