#pragma once

#include "Daydream/Graphics/Core/RenderCommandList.h"
#include "VulkanRenderDevice.h"

namespace Daydream
{
	class VulkanCommandList : public RenderCommandList
	{
	public:
		VulkanCommandList(VulkanRenderDevice* _device);
		~VulkanCommandList();

		vk::CommandBuffer GetVkCommandBuffer() const { return commandBuffer.get(); }
	protected:

	private:
		VulkanRenderDevice* device;
		vk::UniqueCommandBuffer commandBuffer;
	};
}
