#include "DaydreamPCH.h"
#include "OpenGLTextureView.h"

namespace Daydream
{
	
	OpenGLTextureView::OpenGLTextureView(Shared<OpenGLGPUTexture> _texture, const TextureViewDesc& _desc)
		:TextureView(_texture, _desc)
	{
	}

}

