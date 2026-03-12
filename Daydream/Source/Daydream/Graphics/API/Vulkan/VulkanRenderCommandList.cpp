#include "DaydreamPCH.h"
#include "VulkanCommandList.h"

namespace Daydream
{
	VulkanCommandList::VulkanCommandList(VulkanRenderDevice* _device)
	{
		device = _device;

		vk::CommandBufferAllocateInfo allocInfo{};
		allocInfo.commandPool = device->GetCommandPool();
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandBufferCount = 1;

		commandBuffer = std::move(device->GetDevice().allocateCommandBuffersUnique(allocInfo).front());
	}

	VulkanCommandList::~VulkanCommandList()
	{
	}
}

