#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/Resources/Sampler.h"
#include "Daydream/Graphics/Resources/GPUResource.h"
#include "Daydream/Asset/Asset.h"

namespace Daydream
{
	// Texture
	enum class TextureType
	{
		Unknown,

		// 1D (선형 데이터)
		Texture1D,
		Texture1DArray,

		// 2D (일반적인 이미지)
		Texture2D,
		Texture2DArray,
		Texture2DMultisample,

		// 3D & Cube (볼륨 및 환경)
		TextureCube,
		TextureCubeArray,
		Texture3D
	};

	struct Texture2DDesc
	{
		UInt32 width = 1;
		UInt32 height = 1;
		UInt32 mipLevels = 1;
		UInt32 sampleCount = 1;
		RenderFormat format;
		TextureUsage textureUsage;
	};


	struct TextureDesc
	{
		UInt32 width = 1;
		UInt32 height = 1;
		UInt32 layerCount = 1;
		UInt32 mipLevels = 1;
		UInt32 sampleCount = 1;
		RenderFormat format;
		TextureUsage textureUsage;
		TextureType type = TextureType::Unknown;
	};

	class GPUTexture : public GPUResource
	{
	public:
		GPUTexture(const TextureDesc& _desc);
		virtual ~GPUTexture() = default;

		const TextureDesc& GetDesc() const { return desc; }

	protected:
		TextureDesc desc;
	};

	class Texture : public Asset
	{
	public:
		Texture(Shared<GPUTexture> _texture) : gpuTexture(_texture) {}
		virtual ~Texture() = default;

		inline UInt32 GetWidth() const { return gpuTexture->GetDesc().width; }
		inline UInt32 GetHeight() const { return gpuTexture->GetDesc().height; }
		inline UInt32 GetMipLevels() const { return gpuTexture->GetDesc().mipLevels; }
		inline UInt32 GetLayerCount() const { return gpuTexture->GetDesc().layerCount; }

		inline Shared<GPUTexture> GetGPUTexture() const { return gpuTexture; }
		inline GPUTexture* GetGPUTexturePtr() const { return gpuTexture.get(); }

		inline TextureDesc GetDesc() const { return gpuTexture->GetDesc(); }
		inline TextureType GetType() const { return gpuTexture->GetDesc().type; }
	protected:
		Shared<GPUTexture> gpuTexture = nullptr;
	};




}