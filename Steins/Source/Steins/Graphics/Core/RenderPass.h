#pragma once

#include "Steins/Enum/RendererEnums.h"

namespace Steins
{
	class Framebuffer;
	struct FramebufferDesc;

	enum class AttachmentLoadOp {
		Load,      // 이전 내용을 그대로 로드
		Clear,     // 특정 값으로 초기화
		DontCare   // 이전 내용 무시 (성능 최적화에 유리)
	};

	enum class AttachmentStoreOp {
		Store,     // 렌더링 결과를 저장
		DontCare   // 렌더링 결과를 버림 (임시 데이터에 유용)
	};

	struct RenderPassAttachmentDesc
	{
		RenderFormat format = RenderFormat::UNKNOWN;
		// 이 Attachment에 대한 처리 방식을 명시	
		AttachmentLoadOp  loadOp = AttachmentLoadOp::Clear;
		AttachmentStoreOp storeOp = AttachmentStoreOp::Store;
		// loadOp가 Clear일 경우 사용할 값
		Color clearValue = Color(0.0f, 0.0f, 0.0f, 1.0f);
	};

	struct RenderPassDesc
	{
		Array<RenderPassAttachmentDesc> colorAttachments;
		RenderPassAttachmentDesc depthAttachment;
		UInt32 samples = 1;
		bool isSwapchain = false;
	};

	class RenderPass
	{
	public:
		virtual ~RenderPass() = default;

		virtual void Begin(Shared<Framebuffer> _framebuffer) = 0;
		virtual void End() = 0;

		virtual Shared<Framebuffer> CreateFramebuffer(const FramebufferDesc& _desc) = 0;

		const RenderPassDesc& GetDesc() { return desc; }

		static Shared<RenderPass> Create(const RenderPassDesc& _desc);
	protected:
		RenderPassDesc desc;



	};
}