#include "DaydreamPCH.h"
#include "VulkanRenderContext.h"


namespace Daydream
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
