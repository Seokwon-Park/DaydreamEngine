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
		OpenGLFramebuffer(OpenGLSwapchain* _swapchain, RenderPass* _renderPass);
		virtual ~OpenGLFramebuffer() override;

		virtual Shared<Texture2D> GetColorAttachmentTexture(UInt32 _index) override;
		virtual Shared<Texture2D> GetDepthAttachmentTexture() override;
		virtual inline bool HasDepthAttachment() override { return depthAttachment != nullptr; }
		virtual void Recreate(UInt32 _newWidth, UInt32 _newHeight) override;
		virtual UInt32 ReadEntityHandleFromPixel(Int32 _mouseX, Int32 _mouseY) override;

		void AttachTextures();
		UInt32 GetFramebufferID() { return framebufferID; }
		void CreateAttachments();
	private:
		UInt32 framebufferID;
		Array<Shared<OpenGLTexture2D>> colorAttachments;
		Shared<OpenGLTexture2D> depthAttachment;

		Shared<OpenGLTexture2D> entityTexture;
	};
}