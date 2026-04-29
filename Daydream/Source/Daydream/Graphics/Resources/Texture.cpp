#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Texture.h"

#include "Daydream/Core/Application.h"

#include "Daydream/Graphics/Core/Renderer.h"

#include "Daydream/Graphics/Utility/ImageLoader.h"
#include "Daydream/Graphics/Utility/GraphicsUtility.h"


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

	Texture2D::Texture2D(Shared<GPUTexture> _texture)
		:Texture(_texture)
	{

	}

	Shared<Texture2D> Texture2D::Create(const void* _data, const TextureDesc& _desc)
	{
		TextureDesc desc = _desc;
		desc.type = TextureType::Texture2D;
		Shared<GPUTexture> gpuTexture = Renderer::GetRenderDevice()->CreateGPUTexture(desc);
		Shared<Texture2D> texture2D = MakeShared<Texture2D>(gpuTexture);

		UInt32 imageSize = _desc.width * _desc.height * GraphicsUtility::GetRenderFormatSize(_desc.format);
		Shared<UploadBuffer> uploadBuffer = UploadBuffer::Create(imageSize);
		uploadBuffer->UpdateData(_data, imageSize);

		Renderer::EnqueuePreFrameCommand([=]()
			{
				Renderer::CopyBufferToTexture(uploadBuffer->GetBuffer(), texture2D->GetTexture(), desc.width, desc.height);
				Renderer::TransitionTextureState(gpuTexture, ResourceState::CopyDest, ResourceState::VertexBuffer, 0, 1);
			});

		return vertexBuffer;
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
		TextureDesc desc = _desc;
		desc.type = TextureType::Texture2D;


		return Renderer::GetRenderDevice()->CreateEmptyTexture2D(_desc);
	}

	TextureCube::TextureCube(Shared<GPUTexture> _texture)
		:Texture(_texture)
	{
	}

	Shared<TextureCube> TextureCube::Create(const Array<Path>& _paths, const TextureDesc& _desc)
	{
		Array<const void*> imageDatas;
		Array<ImageData> temp;
		TextureDesc finalDesc = _desc;
		for (auto path : _paths)
		{
			ImageData imageData = ImageLoader::LoadImageFile(path);

			if (std::get<Array<UInt8>>(imageData.data).data() != nullptr)
			{
				finalDesc.width = imageData.width;
				finalDesc.height = imageData.height;

				temp.push_back(imageData);
			}
		}
		for (int i = 0; i < temp.size(); i++)
		{
			imageDatas.push_back(std::get<Array<UInt8>>(temp[i].data).data());
		}
		Shared<TextureCube> textureCube = Renderer::GetRenderDevice()->CreateTextureCube(imageDatas, finalDesc);
		for (int i = 0; i < 6; i++)
		{
			Path path = _paths[i];
			textureCube->textures[i] = ResourceManager::GetResource<Texture2D>(path.make_preferred().string());
		}
		return textureCube;
	}

	Shared<TextureCube> TextureCube::Create(const Array<Shared<Texture2D>>& _textures, const TextureDesc& _desc)
	{
		TextureDesc desc = _desc;
		desc.type = TextureType::Texture2D;
		Shared<GPUTexture> gpuTexture = Renderer::GetRenderDevice()->CreateGPUTexture(desc);
		textureCube->textures = _textures;
		return textureCube;
	}

	Shared<TextureCube> TextureCube::CreateEmpty(const TextureDesc& _desc)
	{
		TextureDesc desc = _desc;
		desc.type = TextureType::Texture2D;
		Shared<GPUTexture> gpuTexture = Renderer::GetRenderDevice()->CreateGPUTexture(desc);
		Shared<TextureCube> textureCube = MakeShared<TextureCube>(gpuTexture);
		return textureCube;
	}

}