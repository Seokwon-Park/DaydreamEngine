#include "SteinsPCH.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Steins
{
	OpenGLFramebuffer::OpenGLFramebuffer(RenderPass* _renderPass, const FramebufferDesc& _desc)
	{
		glCreateFramebuffers(1, &framebufferID);

		width = _desc.width;
		height = _desc.height;

		const RenderPassDesc& renderPassDesc = _renderPass->GetDesc();
		for (const auto& colorAttachmentDesc : renderPassDesc.colorAttachments)
		{
			TextureDesc textureDesc;
			textureDesc.width = _desc.width;
			textureDesc.height = _desc.height;
			textureDesc.format = colorAttachmentDesc.format;
			textureDesc.bindFlags = RenderBindFlags::RenderTarget | RenderBindFlags::ShaderResource;

			Shared<OpenGLTexture2D> colorTexture = MakeShared<OpenGLTexture2D>(textureDesc);
			colorAttachments.push_back(colorTexture);
		}

		if (renderPassDesc.depthAttachment.format != RenderFormat::UNKNOWN)
		{
			TextureDesc textureDesc;
			textureDesc.width = _desc.width;
			textureDesc.height = _desc.height;
			textureDesc.format = renderPassDesc.depthAttachment.format;
			textureDesc.bindFlags = RenderBindFlags::DepthStencil;

			Shared<OpenGLTexture2D> depthTexture = MakeShared<OpenGLTexture2D>(textureDesc);
			depthAttachment = depthTexture;
		}

		AttachTextures();
	}


	OpenGLFramebuffer::OpenGLFramebuffer(OpenGLSwapchain* _desc)
	{
		isBackbuffer = true;
	}

	void OpenGLFramebuffer::AttachTextures()
	{
		// Color attachments 연결
		for (UInt32 i = 0; i < colorAttachments.size(); ++i) {
			glNamedFramebufferTexture(framebufferID,
				GL_COLOR_ATTACHMENT0 + i,
				colorAttachments[i]->GetTextureID(),
				0); // mip level 0
		}

		// Depth attachment 연결
		if (depthAttachment) {
			GLenum attachment = GL_DEPTH_STENCIL_ATTACHMENT;

			glNamedFramebufferTexture(framebufferID,
				attachment,
				depthAttachment->GetTextureID(),
				0);
		}

		// Draw buffers 설정 (multiple render targets용)
		std::vector<GLenum> drawBuffers;
		for (UInt32 i = 0; i < colorAttachments.size(); ++i) {
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
		glNamedFramebufferDrawBuffers(framebufferID,
			drawBuffers.size(),
			drawBuffers.data());

		//glNamedFramebufferDrawBuffer(framebufferID, GL_COLOR_ATTACHMENT0);

		STEINS_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
	}
	Shared<Texture2D> OpenGLFramebuffer::GetColorAttachmentTexture(UInt32 _index)
	{
		return colorAttachments[_index];
	}
}
