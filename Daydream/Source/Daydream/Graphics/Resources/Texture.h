#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/Resources/Sampler.h"
#include "Daydream/Asset/Asset.h"

namespace Daydream
{

	// Texture
	enum class TextureType {
		Unknown,
		Texture2D,
		TextureCube,
		Texture3D
	};

	struct TextureDesc
	{
		UInt32 width;
		UInt32 height;
		UInt32 mipLevels = 1;
		RenderFormat format;
		RenderBindFlags bindFlags;
		TextureType type = TextureType::Unknown;
	};


	class Texture : public Asset
	{
	public:
		Texture(const TextureDesc& _desc);
		virtual ~Texture() = default;

		UInt32 GetWidth() const { return desc.width; }
		UInt32 GetHeight() const { return desc.height; }
		UInt32 GetMipLevels() const { return desc.mipLevels; }
		virtual UInt32 GetLayerCount() const = 0;


		virtual void SetSampler(Shared<Sampler> _sampler) = 0;
		virtual bool HasSampler() = 0;

		virtual void* GetNativeHandle() = 0;
		//virtual void* GetImGuiHandle() = 0;

		TextureDesc GetDesc() const { return desc; }
		TextureType GetType() const { return desc.type; }
	protected:
		TextureDesc desc = TextureDesc();
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const TextureDesc& _desc);
		virtual ~Texture2D() = default;

		
		virtual UInt32 GetLayerCount() const { return 1; }

		virtual void* GetNativeHandle() = 0;
		virtual void* GetImGuiHandle() = 0;

		static Shared<Texture2D> Create(const void* _data, const TextureDesc& _desc);
		static Shared<Texture2D> CreateFromFile(const Path& _path, const TextureDesc& _desc);
		static Shared<Texture2D> CreateEmpty(const TextureDesc& _desc);
	protected:
		Texture2D() = default;

		UInt32 channels = 0;
	};


}