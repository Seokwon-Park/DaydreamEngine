#include "SteinsPCH.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Steins
{
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferDesc& _desc)
	{
		glCreateFramebuffers(1, &framebufferID);
		
		//glTextureStorage2D();

		//glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
		//glBindTexture(GL_TEXTURE_2D, colorAttachment);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		//glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		//glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		//// glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, 
		////		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		//STS_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
	}
}
