#pragma once

#include "Daydream/Graphics/API/OpenGL/OpenGLTexture.h"
#include "Daydream/Graphics/Resources/Texture/TextureView.h"

namespace Daydream
{
	class OpenGLTextureView : public TextureView
	{
	public:
		OpenGLTextureView(Shared<OpenGLGPUTexture> _texture, const TextureViewDesc& _desc);
		~OpenGLTextureView() override = default;

		UInt32 GetTextureID() const { return textureViewID; }

	private:
		TextureViewDesc desc;
		UInt32 textureViewID = 0;
	};
}