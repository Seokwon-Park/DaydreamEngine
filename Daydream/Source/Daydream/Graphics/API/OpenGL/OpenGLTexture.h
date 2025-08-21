#pragma once

#include "Daydream/Graphics/Resources/Texture.h"

namespace Daydream
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureDesc& _desc, const void* _initialData = nullptr);
		virtual ~OpenGLTexture2D() override;

		virtual void* GetNativeHandle() override { return reinterpret_cast<void*>(static_cast<UInt64>(textureID)); }
		virtual void* GetImGuiHandle() { return  reinterpret_cast<void*>(static_cast<UInt64>(textureID)); }

		UInt32 GetTextureID() const { return textureID; }

	private:
		UInt32 textureID;
	};
}