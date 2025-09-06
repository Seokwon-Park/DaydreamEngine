#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Core/Resource.h"

namespace Daydream
{

	// Texture
	enum class TextureType {
		Texture2D,
		TextureCubeMap,
		Texture3D
	};

	struct TextureDesc
	{
		UInt32 width;
		UInt32 height;
		//FilterMode filterMode;
		//WrapMode wrapMode;
		RenderFormat format;
		RenderBindFlags bindFlags;
	};


	class Texture : public Resource
	{
	public:
		virtual ~Texture() = default;

		virtual UInt32 GetWidth() const = 0;
		virtual UInt32 GetHeight() const = 0;

		virtual void* GetNativeHandle() = 0;
		//virtual void* GetImGuiHandle() = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		virtual UInt32 GetWidth() const { return width; }
		virtual UInt32 GetHeight() const { return height; }

		virtual void* GetNativeHandle() = 0;
		virtual void* GetImGuiHandle() = 0;

		static Shared<Texture2D> Create(const Path& _path, const TextureDesc& _desc);
		static Shared<Texture2D> CreateEmpty(const TextureDesc& _desc);
	protected:
		Texture2D() = default;

		TextureDesc desc = TextureDesc();
		Int32 width = 0;
		Int32 height = 0;
		Int32 channels = 0;
	};


}