#pragma once

#include "Texture.h"


namespace Daydream
{
	class TextureCube : public Texture
	{
	public:
		ASSET_CLASS_TYPE(TextureCube)
		TextureCube(const TextureDesc& _dsec);
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