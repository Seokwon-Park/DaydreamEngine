#include "SteinsPCH.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include "glad/glad.h"

namespace Steins
{
	OpenGLTexture2D::OpenGLTexture2D(const FilePath& _path)
	{
		int tmpWidth, tmpHeight, tmpCh;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(_path.ToString().c_str(), &tmpWidth, &tmpHeight, &tmpCh, STBI_rgb_alpha);
		STEINS_CORE_ASSERT(data, "Failed to load image!");

		width = tmpWidth;
		height = tmpHeight;

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, GL_RGBA8, width, height);

		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTextureSubImage2D(rendererID, 1, 0, 0, width, height, GL_RGBA8, GL_UNSIGNED_BYTE, data);

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
