#include "DaydreamPCH.h"
#include "VulkanTextureView.h"

namespace Daydream
{
	VulkanTextureView::VulkanTextureView(VulkanRenderDevice* _device, Shared<VulkanGPUTexture> _texture, const TextureViewDesc& _desc)
		:TextureView(_texture, _desc)
	{
		device = _device;

		vk::ImageViewCreateInfo viewInfo{};
		device->GetDevice().createImageViewUnique(viewInfo);
	}
}

