#pragma once

#include <Daydream/Enum/RendererEnums.h>
#include <Daydream/Graphics/Core/RenderPass.h>
#include <Daydream/Graphics/Resources/Texture.h>

namespace Daydream
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

		virtual Shared<Texture2D> GetColorAttachmentTexture(UInt32 _index) = 0;
		virtual bool HasDepthAttachment() = 0;
		virtual void Resize(UInt32 _width, UInt32 _height) {};
		//virtual Shared<Texture2D> GetDepthAttachemntTexture(UInt32 _index) = 0;

		UInt32 GetWidth() { return width; }
		UInt32 GetHeight() { return height; }
		static Shared<Framebuffer> Create(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc);
	protected:
		UInt32 width;
		UInt32 height;
		RenderPass* renderPass;
	private:
	};
}