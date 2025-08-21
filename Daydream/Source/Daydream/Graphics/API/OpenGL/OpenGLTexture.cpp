#include "DaydreamPCH.h"
#include "OpenGLTexture.h"

#include "glad/glad.h"
#include "Daydream/Graphics/Utility/GraphicsUtil.h"
#include "Daydream/Graphics/Utility/ImageLoader.h"

namespace Daydream
{
	OpenGLTexture2D::OpenGLTexture2D(const TextureDesc& _desc, const void* _initialData)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);

		width = _desc.width;
		height = _desc.height;

		GLenum internalFormat = 0, dataFormat = 0;
		internalFormat = GraphicsUtil::ConvertRenderFormatToGLFormat(_desc.format);
		dataFormat = GraphicsUtil::ConvertRenderFormatToGLDataFormat(_desc.format);

		DAYDREAM_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		// 텍스처 저장공간 할당 (데이터는 nullptr)
		glTextureStorage2D(textureID, 1, internalFormat, width, height);

		// 필터링 설정
		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//Array<UInt8> redData(width * height * 4, 0);
		//for (int i = 0; i < width*height; i++) {
		//	redData[i * 4] = 255;     // R
		//	redData[i * 4 + 1] = 0;     // G  
		//	redData[i * 4 + 2] = 0;     // B
		//	redData[i * 4 + 3] = 255;   // A
		//}
		if (_initialData)
		{
			glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, _initialData);
		}

		//glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, redData.data());

		//glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &textureID);
	}
}
