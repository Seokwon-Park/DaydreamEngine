#pragma once

#include "Daydream/Enum/RendererEnums.h"

namespace Daydream
{
	class Framebuffer;
	struct FramebufferDesc;

	enum class AttachmentLoadOp {
		Load,      // ���� ������ �״�� �ε�
		Clear,     // Ư�� ������ �ʱ�ȭ
		DontCare   // ���� ���� ���� (���� ����ȭ�� ����)
	};

	enum class AttachmentStoreOp {
		Store,     // ������ ����� ����
		DontCare   // ������ ����� ���� (�ӽ� �����Ϳ� ����)
	};

	struct RenderPassAttachmentDesc
	{
		RenderFormat format = RenderFormat::UNKNOWN;
		// �� Attachment�� ���� ó�� ����� ���	
		AttachmentLoadOp  loadOp = AttachmentLoadOp::Clear;
		AttachmentStoreOp storeOp = AttachmentStoreOp::Store;
		// loadOp�� Clear�� ��� ����� ��
		Color clearValue = Color(0.0f, 0.0f, 0.0f, 1.0f);
		bool isSwapchain = false;
	};

	struct RenderPassDesc
	{
		Array<RenderPassAttachmentDesc> colorAttachments;
		RenderPassAttachmentDesc depthAttachment;
		UInt32 samples = 1;
	};

	//TODO : Framebuffer���� Validation �߰��� ��.
	//       �������� �ɼǵ� ��� �߰����� �����غ���
	class RenderPass
	{
	public:
		virtual ~RenderPass() = default;

		virtual void Begin(Shared<Framebuffer> _framebuffer) = 0;
		virtual void End() = 0;

		virtual Shared<Framebuffer> CreateFramebuffer(const FramebufferDesc& _desc) = 0;

		const RenderPassDesc& GetDesc() { return desc; }
		void SetClearColor(Color _clearColor) { clearColor = _clearColor; }

		static Shared<RenderPass> Create(const RenderPassDesc& _desc);
	protected:
		RenderPassDesc desc;
		Color clearColor;
	};
}