#include "SteinsPCH.h"
#include "VUlkanGraphicsContext.h"


namespace Steins
{
	VUlkanGraphicsContext::VUlkanGraphicsContext(VulkanGraphicsDevice* _device)
	{
		device = _device;
	}
	void VUlkanGraphicsContext::Init()
	{
	}
	void VUlkanGraphicsContext::SetClearColor(const Color& _color)
	{
	}

	void VUlkanGraphicsContext::Clear()
	{
		//vkCmdClearAttachments
	}

	void VUlkanGraphicsContext::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		vkCmdDrawIndexed(device->GetCommandBuffer(), _indexCount, 1, _startIndex, _baseVertex, 0);
	}
}
