#include "SteinsPCH.h"
#include "VulkanRendererAPI.h"


namespace Steins
{
	VulkanRendererAPI::VulkanRendererAPI(GraphicsDevice* _device)
	{
		device = Cast<VulkanGraphicsDevice>(_device);
	}
	void VulkanRendererAPI::Init()
	{
	}
	void VulkanRendererAPI::SetClearColor(const Color& _color)
	{
	}

	void VulkanRendererAPI::Clear()
	{
		//vkCmdClearAttachments
	}

	void VulkanRendererAPI::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		vkCmdDrawIndexed(device->GetCommandBuffer(), _indexCount, 1, _startIndex, _baseVertex, 0);
	}
}
