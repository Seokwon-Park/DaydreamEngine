#pragma once

#include "Daydream/Graphics/Resources/RenderPass.h"

namespace Daydream
{
	class OpenGLFramebuffer;

	class OpenGLRenderPass : public RenderPass
	{
	public:
		OpenGLRenderPass(const RenderPassDesc& _desc);
		virtual ~OpenGLRenderPass() {};

		virtual void Begin(Shared<Framebuffer> _framebuffer) override;
		virtual void End() override;

		virtual Shared<Framebuffer> CreateFramebuffer(const FramebufferDesc& _desc) override;
	private:
		Shared<OpenGLFramebuffer> currentFramebuffer;
	};
}