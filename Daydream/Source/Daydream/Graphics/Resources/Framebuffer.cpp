#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Framebuffer.h"

#include "Daydream/Graphics/Core/RenderDevice.h"
#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	void Framebuffer::RequestResize(UInt32 _width, UInt32 _height)
	{
		Renderer::GetRenderDevice()->AddFramebufferResizeRequest(this, Vector2(_width, _height));
	}
	Shared<Framebuffer> Framebuffer::Create(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc)
	{
		return Renderer::GetRenderDevice()->CreateFramebuffer(_renderPass, _desc);
	}
}