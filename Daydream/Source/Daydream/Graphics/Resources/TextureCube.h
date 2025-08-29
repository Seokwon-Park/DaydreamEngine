#pragma once

#include "Texture.h"


namespace Daydream
{
	class TextureCube : public Texture
	{
	public:
		virtual ~TextureCube() = default;


		virtual UInt32 GetWidth() const { return width; }
		virtual UInt32 GetHeight() const { return height; }

		virtual void* GetNativeHandle() override =0 ;

		static Shared<TextureCube> Create(const Array<Path>& _paths, const TextureDesc& _desc);
		static Shared<TextureCube> CreateFromDDS(const Path& _path, const TextureDesc& _desc);
		static Shared<TextureCube> CreateFromEquirectangular(const Path& _path, const TextureDesc& _desc);
	protected:
		TextureDesc desc;
		Int32 width = 0;
		Int32 height = 0;
	};
}