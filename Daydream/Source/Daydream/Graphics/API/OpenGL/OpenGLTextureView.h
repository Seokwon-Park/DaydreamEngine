#pragma once

#include "Daydream/Graphics/Resources/TextureView.h"

namespace Daydream
{
	class OpenGLTextureView : public TextureView
	{
	public:
		explicit OpenGLTextureView(const TextureViewDesc& _desc);
		~OpenGLTextureView() override = default;

		const TextureViewDesc& GetDesc() const override { return desc; }

		void SetTextureID(UInt32 _textureID) { textureID = _textureID; }
		UInt32 GetTextureID() const { return textureID; }

	private:
		TextureViewDesc desc;
		UInt32 textureID = 0;
	};
}