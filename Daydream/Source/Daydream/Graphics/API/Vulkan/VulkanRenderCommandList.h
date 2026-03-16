#pragma once

#include "Daydream/Graphics/Core/RenderCommandList.h"
#include "VulkanRenderDevice.h"

namespace Daydream
{
	class VulkanRenderCommandList : public RenderCommandList
	{
	public:
		VulkanRenderCommandList(VulkanRenderDevice* _device);
		~VulkanRenderCommandList();

		virtual void Begin() override;
		virtual void End() override;
		virtual void WaitForCompletion() override;

		vk::CommandBuffer GetVkCommandBuffer() const { return commandBuffer.get(); }
		vk::Fence GetVkFence() const { return inFlightFence.get(); }
	protected:

	private:
		VulkanRenderDevice* device;
		vk::UniqueCommandBuffer commandBuffer;
		vk::UniqueFence inFlightFence;
	};
}