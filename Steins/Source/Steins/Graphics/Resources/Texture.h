#pragma once

namespace Steins
{
	class Texture
	{
	public :
		virtual ~Texture() = default;

		virtual UInt32 GetWidth() const = 0;
		virtual UInt32 GetHeight() const = 0;

		virtual void Bind(UInt32 _slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const FilePath& _path); 
		virtual ~Texture2D() = default;

		virtual UInt32 GetWidth() const { return width; }
		virtual UInt32 GetHeight() const { return height; }

		virtual void* GetNativeHandle() = 0;

		static Shared<Texture2D> Create(const FilePath& _path);
	protected:
		unsigned char* data;
		FilePath path;
		Int32 width;
		Int32 height;
		Int32 channels;
	};
}