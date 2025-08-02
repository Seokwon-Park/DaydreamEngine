#pragma once

#include "Daydream/Graphics/Resources/Framebuffer.h"
#include "OpenGLTexture.h"

namespace Daydream
{
	class OpenGLSwapchain;

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(RenderPass* _renderPass, const FramebufferDesc& _desc);
		OpenGLFramebuffer(OpenGLSwapchain* _desc);
		virtual ~OpenGLFramebuffer() override;

		virtual Shared<Texture2D> GetColorAttachmentTexture(UInt32 _index) override;
		virtual inline bool HasDepthAttachment() override { return depthAttachment != nullptr; }
		virtual void Resize(UInt32 _width, UInt32 _height) override;

		void AttachTextures();
		bool IsBackbuffer() { return isBackbuffer; }
		UInt32 GetFramebufferID() { return framebufferID; }
		void CreateAttachments();
	private:
		UInt32 framebufferID;
		Array<Shared<OpenGLTexture2D>> colorAttachments;
		Shared<OpenGLTexture2D> depthAttachment;
		bool isBackbuffer = false;
	};
}