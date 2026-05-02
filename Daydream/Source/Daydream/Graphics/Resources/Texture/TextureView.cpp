#include "DaydreamPCH.h"
#include "TextureView.h"

#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{

	TextureView::TextureView(Shared<GPUTexture> _texture, const TextureViewDesc& _desc)
	{
		desc = _desc;
		originTexture = _texture;
	}

	TextureView::~TextureView()
	{
		originTexture = nullptr;
	}

	Shared<TextureView> TextureView::Create(Shared<Texture2D> _texture, const TextureViewDesc& _desc)
	{
		return Renderer::GetRenderDevice()->CreateTextureView(_texture, _desc);
	}
}

