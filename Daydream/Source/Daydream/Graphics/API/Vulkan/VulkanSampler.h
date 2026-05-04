#pragma once

#include "VulkanRenderDevice.h"
#include "Daydream/Graphics/Resources/Sampler.h"

namespace Daydream
{
	class VulkanSampler : public Sampler
	{
	public:
		VulkanSampler(VulkanRenderDevice* _device, const SamplerDesc& _desc);
		virtual ~VulkanSampler();

		inline vk::Sampler GetVkSampler() const { return sampler.get(); }
	private:
		VulkanRenderDevice* device;
		vk::UniqueSampler sampler;
	};
}
