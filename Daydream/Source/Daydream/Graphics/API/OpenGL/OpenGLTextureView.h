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

		void SetTextureID(UInt32 _textureID) { textureID = _textureID; }
		UInt32 GetTextureID() const { return textureID; }

	private:
		TextureViewDesc desc;
		UInt32 textureID = 0;
	};
}