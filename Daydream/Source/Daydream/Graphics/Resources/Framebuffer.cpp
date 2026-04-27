#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Framebuffer.h"

#include "Daydream/Graphics/Core/RenderDevice.h"
#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Core/Swapchain.h"

namespace Daydream
{
	Framebuffer::Framebuffer(Swapchain* _swapchain, RenderPass* _renderPass)
	{
		width = _swapchain->GetDesc().width;
		height = _swapchain->GetDesc().height;

		renderPass = _renderPass;
		colorAttachmentCount = (UInt32)_renderPass->GetDesc().colorAttachments.size();
	}
	Framebuffer::Framebuffer(RenderPass* _renderPass, const FramebufferDesc& _desc)
	{
		width = _desc.width;
		height = _desc.height;

		renderPass = _renderPass;
		colorAttachmentCount = (UInt32)_renderPass->GetDesc().colorAttachments.size();
	}
	void Framebuffer::SetSize(UInt32 _width, UInt32 _height)
	{
		width = _width;
		height = _height;
	}

	Shared<Framebuffer> Framebuffer::Create(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc)
	{
		return Renderer::GetRenderDevice()->CreateFramebuffer(_renderPass, _desc);
	}
}