#include "SteinsPCH.h"
#include "VulkanGraphicsContext.h"


namespace Steins
{
	VulkanGraphicsContext::VulkanGraphicsContext(VulkanRenderDevice* _device)
	{
		device = _device;
	}
	void VulkanGraphicsContext::Init()
	{
	}
	void VulkanGraphicsContext::SetClearColor(const Color& _color)
	{
	}

	void VulkanGraphicsContext::Clear()
	{
		//vkCmdClearAttachments
	}

	void VulkanGraphicsContext::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		vkCmdDrawIndexed(device->GetCommandBuffer(), _indexCount, 1, _startIndex, _baseVertex, 0);
	}
}
