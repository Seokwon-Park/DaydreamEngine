#include "DaydreamPCH.h"
#include "OpenGLTextureView.h"

#include "glad/glad.h"
#include "OpenGLUtility.h"

namespace Daydream
{
	OpenGLTextureView::OpenGLTextureView(Shared<OpenGLGPUTexture> _texture, const TextureViewDesc& _desc)
		:TextureView(_texture, _desc)
	{
		glGenTextures(1, &textureViewID);

		GLenum target = GraphicsUtility::OpenGL::ConvertToOpenGLTextureTarget(_texture->GetDesc().type);
		GLenum internalFormat = GraphicsUtility::OpenGL::ConvertRenderFormatToGLFormat(_texture->GetDesc().format);

		glTextureView(textureViewID, target, _texture->GetTextureID(), internalFormat, _desc.baseMip, _desc.mipCount, _desc.baseLayer, _desc.layerCount);
	}
}

