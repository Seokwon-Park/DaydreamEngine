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

		static Shared<TextureCube> Create(const Texture2DDesc& _desc);
	protected:

	};
}