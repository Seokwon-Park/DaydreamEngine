#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/Resources/Texture/TextureView.h"

namespace Daydream
{
	//struct FramebufferDesc
	//{
	//	UInt32 width = 0;
	//	UInt32 height = 0;

	//	Array<Shared<TextureView>> colorAttachments;
	//	Shared<TextureView> depthAttachment = nullptr;

	//	bool isSwapchainBuffer = false;
	//	String debugName = "Framebuffer"; 
	//};

	//class Swapchain;

	//class Framebuffer
	//{
	//public:
	//	Framebuffer(const FramebufferDesc& _desc);
	//	virtual ~Framebuffer() = default;

	//	inline UInt64 GetColorAttachmentSize() { return colorAttachments.size(); };
	//	inline Shared<Texture2D> GetColorAttachment(UInt32 _index) {};
	//	inline Shared<Texture2D> GetDepthAttachment() {};
	//	bool HasDepthAttachment() { return depthAttachment != nullptr; }
	//	//virtual void Recreate(UInt32 _newWidth, UInt32 _newHeight) = 0;

	//	//inline UInt32 ReadEntityHandleFromPixel(Pair<Int32, Int32> _mousePos)
	//	//{
	//	//	return ReadEntityHandleFromPixel(_mousePos.first, _mousePos.second);
	//	//}
	//	//virtual UInt32 ReadEntityHandleFromPixel(Int32 _mouseX, Int32 _mouseY) { return 0; };
	//	////virtual Shared<Texture2D> GetDepthAttachemntTexture(UInt32 _index) = 0;

	//	//bool IsSwapchainBuffer() const { return isSwapchainBuffer; }

	//	UInt32 GetWidth() { return width; }
	//	UInt32 GetHeight() { return height; }
	//	static Shared<Framebuffer> Create(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc);
	//protected:
	//	void SetSize(UInt32 _width, UInt32 _height);
	//	RenderPass* renderPass;

	//	bool isSwapchainBuffer = false;
	//	UInt32 width;
	//	UInt32 height;

	//	Array<Shared<TextureView>> colorAttachments;
	//	Shared<TextureView> depthAttachment;

	//	UInt32 colorAttachmentCount = 0;
	//private:
	//};
}