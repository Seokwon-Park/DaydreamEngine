#pragma once

#include "Steins/Graphics/Resources/Framebuffer.h"
#include "OpenGLTexture.h"

namespace Steins
{
	class OpenGLSwapchain;

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(RenderPass* _renderPass, const FramebufferDesc& _desc);
		OpenGLFramebuffer(OpenGLSwapchain* _desc);
		virtual ~OpenGLFramebuffer() override;

		virtual void* GetColorAttachmentTexture(UInt32 _index) override;

		void AttachTextures();
		bool IsBackbuffer() { return isBackbuffer; }
		UInt32 GetFramebufferID() { return framebufferID; }
	private:
		UInt32 framebufferID;
		Array<Shared<OpenGLTexture2D>> colorAttachments;
		Shared<OpenGLTexture2D> depthAttachment;
		bool isBackbuffer = false;
	};
}