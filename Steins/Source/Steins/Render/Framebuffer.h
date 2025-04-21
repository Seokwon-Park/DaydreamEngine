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

	struct FramebufferAttachmentSpecification
	{
		FramebufferTextureFormat format = FramebufferTextureFormat::None;
	};

	struct FramebufferSpecification
	{
		UInt32 width, height;
		std::vector<FramebufferAttachmentSpecification> colorAttachments;
		FramebufferAttachmentSpecification depthAttachment;
		UInt32 samples = 1;
		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;

		static Shared<Framebuffer> Create(const FramebufferSpecification& _spec);
	private:
	};
}