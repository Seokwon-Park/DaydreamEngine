#include "DaydreamPCH.h"
#include "OpenGLRenderPass.h"
#include "OpenGLFramebuffer.h"

#include "glad/glad.h"

namespace Daydream
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
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else
		{
			glViewport(0, 0, currentFramebuffer->GetWidth(), currentFramebuffer->GetHeight());
			glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer->GetFramebufferID());
			if (currentFramebuffer->HasDepthAttachment())
			{
				glEnable(GL_DEPTH_TEST);
				glClear(GL_DEPTH_BUFFER_BIT);
			}

			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
	void OpenGLRenderPass::End()
	{
		if (currentFramebuffer->HasDepthAttachment())
		{
			glDisable(GL_DEPTH_TEST);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	Shared<Framebuffer> OpenGLRenderPass::CreateFramebuffer(const FramebufferDesc& _desc)
	{
		return MakeShared<OpenGLFramebuffer>(this, _desc);
	}
}
