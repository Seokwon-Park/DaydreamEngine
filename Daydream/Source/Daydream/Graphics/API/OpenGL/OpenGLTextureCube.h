#pragma once

#include "Daydream/Graphics/Resources/TextureCube.h"

namespace Daydream
{
	class OpenGLTextureCube : public TextureCube
	{
	public:
		// Constrcuter Destructer
		OpenGLTextureCube(const TextureDesc& _desc, const Array<Array<UInt8>>& _initialData);
		virtual ~OpenGLTextureCube();
		virtual inline void* GetNativeHandle() override { return reinterpret_cast<void*>(static_cast<UInt64>(textureID)); }
	private:
		UInt32 textureID;
	};
}