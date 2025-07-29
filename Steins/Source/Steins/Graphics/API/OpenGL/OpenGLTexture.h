#pragma once

#include "Steins/Graphics/Resources/Texture.h"

namespace Steins
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureDesc& _desc);
		OpenGLTexture2D(const FilePath& _path, const TextureDesc& _desc);
		virtual ~OpenGLTexture2D() override;

		virtual void* GetNativeHandle() override { return reinterpret_cast<void*>(static_cast<UInt64>(textureID)); }
		virtual void* GetImGuiHandle() { return  reinterpret_cast<void*>(static_cast<UInt64>(textureID)); }

		UInt32 GetTextureID() const { return textureID; }

	private:
		UInt32 textureID;
	};
}