#pragma once

#include <Steins/Enum/RendererEnums.h>

namespace Steins
{
	struct FramebufferAttachmentDesc
	{
		RenderFormat format = RenderFormat::UNKNOWN;
	};

	struct FramebufferDesc
	{
		UInt32 width;
		UInt32 height;
		Array<FramebufferAttachmentDesc> colorAttachments;
		FramebufferAttachmentDesc depthAttachment;
		UInt32 samples = 1;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Begin() const = 0;
		virtual void End() const = 0;
		virtual void Clear(Color _color) = 0;

		static Shared<Framebuffer> Create(const FramebufferDesc& _spec);
	protected:
	private:
	};
}