#pragma once

#include "Steins/Enum/RendererEnums.h"

namespace Steins
{

	// Texture
	enum class TextureType {
		Texture2D,
		TextureCubeMap,
		Texture3D
	};

	enum class DataType {
		UnsignedByte,
		Float,
		UnsignedInt
	};

	enum class FilterMode {
		LINEAR,
		NEAREST,
		LINEAR_MIPMAP_LINEAR
	};

	enum class WrapMode {
		REPEAT,
		ClampToEdge,
		MirroredRepeat
	};

	struct TextureDesc
	{
		UInt32 width;
		UInt32 height;
		TextureType type;
		FilterMode filterMode;
		WrapMode wrapMode;
		RenderFormat format;
		RenderBindFlags bindFlags;
	};


	class Texture
	{
	public :
		virtual ~Texture() = default;

		virtual UInt32 GetWidth() const = 0;
		virtual UInt32 GetHeight() const = 0;

		virtual void* GetNativeHandle() = 0;
		virtual void* GetImGuiHandle() = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D() {};
		Texture2D(const FilePath& _path);
		virtual ~Texture2D() = default;

		virtual UInt32 GetWidth() const { return width; }
		virtual UInt32 GetHeight() const { return height; }

		virtual void* GetNativeHandle() override = 0;
		virtual void* GetImGuiHandle() override = 0;

		static Shared<Texture2D> Create(const FilePath& _path, const TextureDesc& _desc);
	protected:
		FilePath path;
		TextureDesc desc;
		Int32 width = 0;
		Int32 height = 0;
		Int32 channels = 0;
	};
}