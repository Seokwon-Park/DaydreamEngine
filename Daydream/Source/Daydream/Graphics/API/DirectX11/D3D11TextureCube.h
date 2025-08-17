#pragma once

#pragma once

#include "D3D11Texture.h"
#include "Daydream/Graphics/Resources/TextureCube.h"


namespace Daydream
{
	class D3D11TextureCube : public TextureCube
	{
	public:
		D3D11TextureCube(const FilePath& _path);
		D3D11TextureCube(const Array<FilePath>& _paths);

		virtual ~D3D11TextureCube() = default;

		virtual UInt32 GetWidth() const { return width; }
		virtual UInt32 GetHeight() const { return height; }

		virtual void* GetNativeHandle() override = 0;
		virtual void* GetImGuiHandle() override = 0;

		static Shared<TextureCube> Create(const FilePath& _path, const TextureDesc& _desc);
		static Shared<TextureCube> CreateEmpty(const TextureDesc& _desc);
	protected:
		FilePath path;
		TextureDesc desc;
		Int32 width = 0;
		Int32 height = 0;
		Int32 channels = 0;
	};
}