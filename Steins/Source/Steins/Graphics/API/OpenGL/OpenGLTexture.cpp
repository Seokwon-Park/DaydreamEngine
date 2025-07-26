#include "SteinsPCH.h"
#include "OpenGLTexture.h"

#include "glad/glad.h"
#include "Steins/Graphics/Utility/ImageLoader.h"

namespace Steins
{
	OpenGLTexture2D::OpenGLTexture2D(const FilePath& _path, const TextureDesc& _desc)
		:Texture2D(_path)
	{
		//stbi_set_flip_vertically_on_load(1);
		Array<UInt8> imageData = ImageLoader::LoadImageFile(_path, width, height, channels);
		STEINS_CORE_ASSERT(!imageData.empty(), "Failed to load image!");

		GLenum internalFormat = 0, dataFormat = 0;
		internalFormat = GL_SRGB8_ALPHA8;
		dataFormat = GL_RGBA;
		
		STEINS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
		glTextureStorage2D(textureID, 1, internalFormat, width, height);

		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, imageData.data());
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &textureID);
	}
}
