#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/Resources/Sampler.h"
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

	class GPUTexture
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
		Texture(Shared<GPUTexture> _texture) : texture(_texture) {}
		virtual ~Texture() = default;

		UInt32 GetWidth() const { return texture->GetDesc().width; }
		UInt32 GetHeight() const { return texture->GetDesc().height; }
		UInt32 GetMipLevels() const { return texture->GetDesc().mipLevels; }
		UInt32 GetLayerCount() const { return texture->GetDesc().layerCount; }

		virtual void SetSampler(Shared<Sampler> _sampler) = 0;
		virtual bool HasSampler() = 0;

		TextureDesc GetDesc() const { return texture->GetDesc(); }
		TextureType GetType() const { return texture->GetDesc().type; }
	protected:
		Shared<GPUTexture> texture;
	};

	class Texture2D : public Texture
	{
	public:
		ASSET_CLASS_TYPE(Texture2D)
		Texture2D(Shared<GPUTexture> _texture);
		virtual ~Texture2D() = default;

		virtual void* GetImGuiHandle() = 0;

		static Shared<Texture2D> Create(const void* _data, const TextureDesc& _desc);
		static Shared<Texture2D> CreateFromFile(const Path& _path, const TextureDesc& _desc);
		static Shared<Texture2D> CreateEmpty(const TextureDesc& _desc);
	protected:
		Texture2D() = default;

		UInt32 channels = 0;
	};


}