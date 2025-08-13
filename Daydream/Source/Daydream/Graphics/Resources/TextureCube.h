#pragma once

#include "Texture.h"

namespace Daydream
{
	class TextureCube : public Texture
	{
	public:
		TextureCube() {};
		TextureCube(const FilePath& _path);
		TextureCube(const Array<FilePath>& _paths);

		virtual ~TextureCube() = default;

		virtual UInt32 GetWidth() const { return width; }
		virtual UInt32 GetHeight() const { return height; }

		virtual void* GetNativeHandle() override = 0;
		virtual void* GetImGuiHandle() override = 0;

		static Shared<Texture2D> Create(const FilePath& _path, const TextureDesc& _desc);
		static Shared<Texture2D> CreateEmpty(const TextureDesc& _desc);
	protected:
		FilePath path;
		TextureDesc desc;
		Int32 width = 0;
		Int32 height = 0;
		Int32 channels = 0;
	};
}