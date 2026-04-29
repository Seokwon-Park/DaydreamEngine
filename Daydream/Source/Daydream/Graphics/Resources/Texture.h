#pragma once

#include "GPUResource.h"
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
		Texture(Shared<GPUTexture> _texture) : texture(_texture) {}
		virtual ~Texture() = default;

		inline UInt32 GetWidth() const { return texture->GetDesc().width; }
		inline UInt32 GetHeight() const { return texture->GetDesc().height; }
		inline UInt32 GetMipLevels() const { return texture->GetDesc().mipLevels; }
		inline UInt32 GetLayerCount() const { return texture->GetDesc().layerCount; }

		inline Shared<GPUTexture> GetTexture() const { return texture; }
		inline GPUTexture* GetTextureRaw() const { return texture.get(); }

		inline TextureDesc GetDesc() const { return texture->GetDesc(); }
		inline TextureType GetType() const { return texture->GetDesc().type; }
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

	class TextureCube : public Texture
	{
	public: 
		ASSET_CLASS_TYPE(TextureCube)
		TextureCube(Shared<GPUTexture> _texture);
		virtual ~TextureCube();

		void Update(UInt32 _faceIndex, Shared<Texture2D> _texture);
		virtual void GenerateMips() {};

		void* GetImGuiHandle(UInt32 _faceIndex);

		static Shared<TextureCube> Create(const Array<Path>& _paths, const TextureDesc& _desc);
		static Shared<TextureCube> Create(const Array<Shared<Texture2D>>& _textures, const TextureDesc& _desc);
		static Shared<TextureCube> CreateEmpty(const TextureDesc& _desc);
	protected:
		Array<Shared<Texture2D>> textures;
	};
}