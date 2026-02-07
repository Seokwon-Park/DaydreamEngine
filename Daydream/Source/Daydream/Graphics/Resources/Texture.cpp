#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Texture.h"

#include "Daydream/Core/Application.h"

#include "Daydream/Graphics/Core/Renderer.h"

#include "Daydream/Graphics/Utility/ImageLoader.h"


namespace Daydream
{
	Texture::Texture(const TextureDesc& _desc)
	{
		desc = _desc;

		UInt32 maxMips = (UInt32)std::floor(std::log2(std::max(_desc.width, _desc.height))) + 1;

		if (desc.mipLevels == 0 || desc.mipLevels > maxMips)
		{
			DAYDREAM_CORE_WARN("mipLevel is to Large {} -> {}", std::max(_desc.width, _desc.height), maxMips);
			desc.mipLevels = maxMips;
		}
	}

	Texture2D::Texture2D(const TextureDesc& _desc)
		:Texture(_desc)
	{

	}

	Shared<Texture2D> Texture2D::Create(const void* _data, const TextureDesc& _desc)
	{
		return Renderer::GetRenderDevice()->CreateTexture2D(_data, _desc);
	}

	Shared<Texture2D> Texture2D::CreateFromFile(const Path& _path, const TextureDesc& _desc)
	{
		String ext = _path.extension().string();
		DAYDREAM_CORE_INFO("{}", ext);
		if (_path.extension().string() == ".png" || _path.extension().string() == ".jpg")
		{
			auto imageData = ImageLoader::LoadImageFile(_path);
			Array<UInt8> pixels = std::get<Array<UInt8>>(imageData.data);
			if (pixels.data() != nullptr)
			{
				TextureDesc finalDesc = _desc;
				finalDesc.width = imageData.width;
				finalDesc.height = imageData.height;

				return Renderer::GetRenderDevice()->CreateTexture2D(pixels.data(), finalDesc);
			}
		}
		else if (_path.extension().string() == ".exr" || _path.extension().string() == ".hdr")
		{
			auto imageData = ImageLoader::LoadImageFile(_path);
			Array<Float32> pixels = std::get<Array<Float32>>(imageData.data);
			if (pixels.data() != nullptr)
			{
				TextureDesc finalDesc = _desc;
				finalDesc.width = imageData.width;
				finalDesc.height = imageData.height;
				finalDesc.format = RenderFormat::R32G32B32A32_FLOAT;
				return Renderer::GetRenderDevice()->CreateTexture2D(pixels.data(), finalDesc);
			}
		}
		else
		{
			return Renderer::GetRenderDevice()->CreateTexture2D(nullptr, _desc);
		}
		return nullptr;
	}

	Shared<Texture2D> Texture2D::CreateEmpty(const TextureDesc& _desc)
	{
		return Renderer::GetRenderDevice()->CreateEmptyTexture2D(_desc);
	}

}