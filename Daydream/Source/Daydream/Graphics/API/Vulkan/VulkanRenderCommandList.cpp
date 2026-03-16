#include "DaydreamPCH.h"
#include "VulkanRenderCommandList.h"

namespace Daydream
{
	VulkanRenderCommandList::VulkanRenderCommandList(VulkanRenderDevice* _device)
	{
		device = _device;

		vk::CommandBufferAllocateInfo allocInfo{};
		allocInfo.commandPool = device->GetCommandPool();
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandBufferCount = 1;

		commandBuffer = std::move(device->GetDevice().allocateCommandBuffersUnique(allocInfo).front());

		vk::FenceCreateInfo fenceInfo{};
		fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
		inFlightFence = device->GetDevice().createFenceUnique(fenceInfo);
	}

	VulkanRenderCommandList::~VulkanRenderCommandList()
	{
	}
	void VulkanRenderCommandList::Begin()
	{
		vk::Result result = device->GetDevice().waitForFences(1, &inFlightFence.get(), VK_FALSE, UINT64_MAX);

		//완료 됐으면 펜스 상태는 신호받기 전으로
		result = device->GetDevice().resetFences(1, &inFlightFence.get());

		commandBuffer->reset({});

		vk::CommandBufferBeginInfo beginInfo{};
		commandBuffer->begin(beginInfo);
	}
	void VulkanRenderCommandList::End()
	{
		commandBuffer->end();
	}

	void VulkanRenderCommandList::WaitForCompletion()
	{
		vk::SubmitInfo submitInfo{};
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer.get();

		vk::Result result = device->GetGraphicsQueue().submit(1, &submitInfo, inFlightFence.get());
		result = device->GetDevice().waitForFences(1, &inFlightFence.get(), VK_FALSE, UINT64_MAX);
	}

}

