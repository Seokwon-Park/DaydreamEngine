#pragma once

#include "Daydream/Graphics/Resources/Texture/TextureView.h"

namespace Daydream
{
	struct ClearValue
	{
		Color colorClearValue{};
		Float32 depthClearValue{};
		UInt8 stencilClearValue{};
	}; 

	struct RenderingDesc
	{
		Shared<TextureView> view; // 여기에 포맷이 이미 내장되어 있음
		AttachmentLoadOp  loadOp = AttachmentLoadOp::Clear;
		AttachmentStoreOp storeOp = AttachmentStoreOp::Store;
		ClearValue clearValue = ClearValue();
	};

	struct RenderingInfo
	{
		Array<RenderingDesc> colorAttachments;
		RenderingDesc depthAttachment;
	};


}
