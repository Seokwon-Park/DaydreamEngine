#pragma once

#include "Texture.h"

namespace Daydream
{
	class TextureCube : public Texture
	{
	public:
		ASSET_CLASS_TYPE(TextureCube)
			TextureCube(Shared<GPUTexture> _texture);
		virtual ~TextureCube();

		//void Update(UInt32 _faceIndex, Shared<Texture2D> _texture);
		//virtual void GenerateMips() {};

		//void* GetImGuiHandle(UInt32 _faceIndex);

		//static Shared<TextureCube> Create(const Array<Path>& _paths, const TextureDesc& _desc);
		//static Shared<TextureCube> Create(const Array<Shared<Texture2D>>& _textures, const TextureDesc& _desc);
		static Shared<TextureCube> Create(const Texture2DDesc& _desc);
	protected:
		/*Array<Shared<Texture2D>> textures;*/
	};
}