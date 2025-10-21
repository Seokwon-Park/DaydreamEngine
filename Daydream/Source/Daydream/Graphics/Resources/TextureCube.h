#pragma once

#include "Texture.h"


namespace Daydream
{
	class TextureCube : public Texture
	{
	public:
		virtual ~TextureCube();

		virtual UInt32 GetWidth() const { return width; }
		virtual UInt32 GetHeight() const { return height; }

		void Update(UInt32 _faceIndex, Shared<Texture2D> _texture);
		virtual void GenerateMips() {};

		virtual void* GetNativeHandle() override = 0;
		void* GetImGuiHandle(UInt32 _faceIndex);

		static Shared<TextureCube> Create(const Array<Path>& _paths, const TextureDesc& _desc);
		static Shared<TextureCube> Create(const Array<Shared<Texture2D>>& _textures, const TextureDesc& _desc);
		static Shared<TextureCube> CreateEmpty(const TextureDesc& _desc);
	protected:
		TextureDesc desc;
		Array<Shared<Texture2D>> textures;
		Int32 width = 0;
		Int32 height = 0;
	};
}