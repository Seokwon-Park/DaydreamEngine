#pragma once

#include <Steins/Enum/RendererEnums.h>

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