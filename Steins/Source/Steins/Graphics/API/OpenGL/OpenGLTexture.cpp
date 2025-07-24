#include "SteinsPCH.h"
#include "OpenGLTexture.h"

#include "glad/glad.h"
#include "stb_image.h"

namespace Steins
{
	OpenGLTexture2D::OpenGLTexture2D(const FilePath& _path, const TextureDesc& _desc)
		:Texture2D(_path)
	{
		//stbi_set_flip_vertically_on_load(1);
		data = stbi_load(_path.ToString().c_str(), &width, &height, &channels, 0);
		STEINS_CORE_ASSERT(data, "Failed to load image!");

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		STEINS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, internalFormat, width, height);

		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(rendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &rendererID);
	}
	void OpenGLTexture2D::Bind(UInt32 slot) const
	{
		glBindTextureUnit(slot, rendererID);
	}
}
