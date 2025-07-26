#pragma once

#include <Steins/Enum/RendererEnums.h>

namespace Steins
{
	enum class AttachmentLoadOp {
		Load,      // 이전 내용을 그대로 로드
		Clear,     // 특정 값으로 초기화
		DontCare   // 이전 내용 무시 (성능 최적화에 유리)
	};

	enum class AttachmentStoreOp {
		Store,     // 렌더링 결과를 저장
		DontCare   // 렌더링 결과를 버림 (임시 데이터에 유용)
	};

	struct FramebufferAttachmentDesc
	{
		RenderFormat format = RenderFormat::UNKNOWN;
		// 이 Attachment에 대한 처리 방식을 명시	
		AttachmentLoadOp  loadOp = AttachmentLoadOp::Clear;
		AttachmentStoreOp storeOp = AttachmentStoreOp::Store;
		// loadOp가 Clear일 경우 사용할 값
		Color clearValue = Color( 0.0f, 0.0f, 0.0f, 1.0f );
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