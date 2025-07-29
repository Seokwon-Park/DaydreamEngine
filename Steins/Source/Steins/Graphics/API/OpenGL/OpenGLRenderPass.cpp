#include "SteinsPCH.h"
#include "OpenGLRenderPass.h"
#include "OpenGLFramebuffer.h"

#include "glad/glad.h"

namespace Steins
{
	OpenGLRenderPass::OpenGLRenderPass(const RenderPassDesc& _desc)
	{
		desc = _desc;
	}
	void OpenGLRenderPass::Begin(Shared<Framebuffer> _framebuffer)
	{
		currentFramebuffer = static_pointer_cast<OpenGLFramebuffer>(_framebuffer);
		if (currentFramebuffer->IsBackbuffer() == true)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			glViewport(0, 0, currentFramebuffer->GetWidth(), currentFramebuffer->GetHeight());
			glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer->GetFramebufferID());

			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
	void OpenGLRenderPass::End()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	Shared<Framebuffer> OpenGLRenderPass::CreateFramebuffer(const FramebufferDesc& _desc)
	{
		return MakeShared<OpenGLFramebuffer>(this, _desc);
	}
}
