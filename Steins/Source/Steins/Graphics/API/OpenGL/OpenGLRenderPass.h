#pragma once

#include "Steins/Graphics/Core/RenderPass.h"

namespace Steins
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