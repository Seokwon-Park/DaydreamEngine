#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Texture/Texture.h"

#include "TextureView.h"

namespace Daydream
{
	GPUTexture::GPUTexture(const TextureDesc& _desc)
		:desc(_desc)
	{
		UInt32 maxMips = (UInt32)std::floor(std::log2(std::max(desc.width, desc.height))) + 1;

		if (desc.mipLevels == 0 || desc.mipLevels > maxMips)
		{
			DAYDREAM_CORE_WARN("mipLevel is to Large {} -> {}", std::max(desc.width, desc.height), maxMips);
			desc.mipLevels = maxMips;
		}
	}
	Shared<TextureView> Texture::GetDefaultSRV() const
	{
		return defaultSRV;
	}
}