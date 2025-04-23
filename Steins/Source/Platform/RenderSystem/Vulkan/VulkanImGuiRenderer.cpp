#include "SteinsPCH.h"
#include "VulkanImGuiRenderer.h"

#include "backends/imgui_impl_vulkan.h"

namespace Steins
{
	VulkanImGuiRenderer::VulkanImGuiRenderer(VulkanGraphicsDevice* _device)
	{
		device = _device;
	}

	void VulkanImGuiRenderer::Init(SteinsWindow* _window)
	{
		ImGuiRenderer::Init(_window);
		ImGui_ImplVulkan_InitInfo info{};
		//info.ApiVersion =;
		info.Instance = device->GetInstance();
		info.PhysicalDevice = device->GetPhysicalDevice();
		info.Device = device->GetLogicalDevice();
		info.QueueFamily = device->GetQueueFamily();
		info.Queue = device->GetQueue();
		info.DescriptorPool = device->GetDescriptorPool();
		info.RenderPass = device->GetRenderPass();
		info.MinImageCount = 2;
		info.ImageCount = 2;
		info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		ImGui_ImplVulkan_Init(&info);
	}
	void VulkanImGuiRenderer::Shutdown()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGuiRenderer::Shutdown();
	}
	void VulkanImGuiRenderer::NewFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGuiRenderer::NewFrame();
	}
	void VulkanImGuiRenderer::Render()
	{
		ImGuiRenderer::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), device->GetCommandBuffer());

	}
}
