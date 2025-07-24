#pragma once

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

	enum TextureUsage : UInt32
	{
		Unknown = 0,
		ShaderResource = 1 << 0,
		RenderTarget = 1 << 1,
		DepthStencil = 1 << 2,
		UnorderdAccess = 1 << 3,
	};

	struct TextureDesc
	{
		TextureType type;
		FilterMode filterMode;
		WrapMode wrapMode;
		UInt32 usage;
	};


	class Texture
	{
	public :
		virtual ~Texture() = default;

		virtual UInt32 GetWidth() const = 0;
		virtual UInt32 GetHeight() const = 0;

		virtual void* GetNativeHandle() = 0;

		virtual void Bind(UInt32 _slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const FilePath& _path);
		virtual ~Texture2D() = default;

		virtual UInt32 GetWidth() const { return width; }
		virtual UInt32 GetHeight() const { return height; }

		virtual void* GetNativeHandle() override = 0;

		static Shared<Texture2D> Create(const FilePath& _path, const TextureDesc& _desc);
	protected:
		unsigned char* data;
		FilePath path;
		TextureDesc desc;
		Int32 width;
		Int32 height;
		Int32 channels;
	};
}