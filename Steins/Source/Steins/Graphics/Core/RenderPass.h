#pragma once

#include "Steins/Enum/RendererEnums.h"
#include "Steins/Graphics/Resources/Framebuffer.h"

namespace Steins
{
	enum class AttachmentLoadOp {
		Load,      // ���� ������ �״�� �ε�
		Clear,     // Ư�� ������ �ʱ�ȭ
		DontCare   // ���� ���� ���� (���� ����ȭ�� ����)
	};

	enum class AttachmentStoreOp {
		Store,     // ������ ����� ����
		DontCare   // ������ ����� ���� (�ӽ� �����Ϳ� ����)
	};

	struct FramebufferAttachmentDesc
	{
		RenderFormat format = RenderFormat::UNKNOWN;
		// �� Attachment�� ���� ó�� ����� ���	
		AttachmentLoadOp  loadOp = AttachmentLoadOp::Clear;
		AttachmentStoreOp storeOp = AttachmentStoreOp::Store;
		// loadOp�� Clear�� ��� ����� ��
		Color clearValue = Color(0.0f, 0.0f, 0.0f, 1.0f);
	};

	struct RenderPassDesc
	{
		Array<FramebufferAttachmentDesc> colorAttachments;
		FramebufferAttachmentDesc depthAttachment;
		UInt32 samples = 1;
	};

	class RenderPass
	{
	public:
		virtual ~RenderPass() = default;

		virtual void Begin(Shared<Framebuffer> _framebuffer) = 0;
		virtual void End() = 0;

		static Shared<RenderPass> Create(const RenderPassDesc& _desc);
	private:
	};
}