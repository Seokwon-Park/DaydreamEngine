#pragma once

#include <Steins/Enum/RendererEnums.h>
#include <Steins/Graphics/Core/RenderPass.h>

namespace Steins
{
	struct FramebufferDesc
	{
		UInt32 width;
		UInt32 height;
		bool isBackBuffer = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void* GetColorAttachmentTexture(UInt32 _index) = 0;

		UInt32 GetWidth() { return width; }
		UInt32 GetHeight() { return height; }
		static Shared<Framebuffer> Create(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc);
	protected:
		UInt32 width;
		UInt32 height;
	private:
	};
}