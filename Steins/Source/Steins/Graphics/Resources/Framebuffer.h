#pragma once

namespace Steins
{
	enum class FramebufferTextureFormat {
		None = 0,
		// Color
		RGBA8,
		R32_INT,
		// Depth/stencil
		DEPTH24STENCIL8,
		Depth = DEPTH24STENCIL8 // Alias
	};

	struct FramebufferAttachmentDescription
	{
		FramebufferTextureFormat format = FramebufferTextureFormat::None;
	};

	struct FramebufferDesc
	{
		UInt32 width, height;
		Array<FramebufferAttachmentDescription> colorAttachments;
		FramebufferAttachmentDescription depthAttachment;
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