#include "SteinsPCH.h"
#include "OpenGLTexture.h"

#include "glad/glad.h"
#include "Steins/Graphics/Utility/GraphicsUtil.h"
#include "Steins/Graphics/Utility/ImageLoader.h"

namespace Steins
{
	OpenGLTexture2D::OpenGLTexture2D(const TextureDesc& _desc)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);

		width = _desc.width;
		height = _desc.height;

		GLenum internalFormat = 0, dataFormat = 0;
		internalFormat = GraphicsUtil::ConvertRenderFormatToGLFormat(_desc.format);
		dataFormat = GL_RGBA;

		STEINS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

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

		//glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, redData.data());

		//glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}
	OpenGLTexture2D::OpenGLTexture2D(const FilePath& _path, const TextureDesc& _desc)
		:Texture2D(_path)
	{
		//stbi_set_flip_vertically_on_load(1);
		Array<UInt8> imageData = ImageLoader::LoadImageFile(_path, width, height, channels);
		STEINS_CORE_ASSERT(!imageData.empty(), "Failed to load image!");

		GLenum internalFormat = 0, dataFormat = 0;
		internalFormat = GraphicsUtil::ConvertRenderFormatToGLFormat(_desc.format);
		dataFormat = GL_RGBA;
		
		//for (int i = 0; i < width * height; i++) {
		//	imageData[i * 4] = 255;     // R
		//	imageData[i * 4 + 1] = 0;     // G  
		//	imageData[i * 4 + 2] = 0;     // B
		//	imageData[i * 4 + 3] = 255;   // A
		//}

		STEINS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
		glTextureStorage2D(textureID, 1, internalFormat, width, height);

		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, imageData.data());
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &textureID);
	}
}
