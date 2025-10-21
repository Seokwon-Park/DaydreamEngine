#include "DaydreamPCH.h"
#include "RenderPassManager.h"

namespace Daydream
{
	RenderPassManager::RenderPassManager()
	{
	}
	void RenderPassManager::CreateEssentialRenderPasses()
	{
		RenderPassDesc rpDesc;

		RenderPassAttachmentDesc attach{};
		attach.format = RenderFormat::R8G8B8A8_UNORM;
		attach.loadOp = AttachmentLoadOp::Clear;
		attach.storeOp = AttachmentStoreOp::Store;
		rpDesc.colorAttachments.push_back(attach);

		attach.format = RenderFormat::D24_UNORM_S8_UINT;
		attach.loadOp = AttachmentLoadOp::Clear;
		attach.storeOp = AttachmentStoreOp::Store;
		rpDesc.depthAttachment = attach;

		registry["StandardRenderPass"] = RenderPass::Create(rpDesc);


		// Mipmap ������ ���� RenderPass (Depth Buffer ����)
		rpDesc.colorAttachments.clear();
		attach.format = RenderFormat::R16G16B16A16_FLOAT; // �Ӹ��� ������ �ؽ�ó ���˿� �����ּ���.
		attach.loadOp = AttachmentLoadOp::Clear; // �� �� ������ �ʿ� �����Ƿ� DontCare�� Clear
		attach.storeOp = AttachmentStoreOp::Store;
		rpDesc.colorAttachments.push_back(attach);

		rpDesc.depthAttachment = {};
		attach.format = RenderFormat::UNKNOWN;
		attach.loadOp = AttachmentLoadOp::Clear;
		attach.storeOp = AttachmentStoreOp::Store;
		rpDesc.depthAttachment = attach;

		registry["MipmapRenderPass"] = RenderPass::Create(rpDesc);
		//rpDesc.colorAttachments.clear();
		//attach.format = RenderFormat::R32G32B32A32_FLOAT;
		//attach.loadOp = AttachmentLoadOp::Clear;
		//attach.storeOp = AttachmentStoreOp::Store;
		//rpDesc.colorAttachments.push_back(attach);

		//attach.format = RenderFormat::D24_UNORM_S8_UINT;
		//attach.loadOp = AttachmentLoadOp::Clear;
		//attach.storeOp = AttachmentStoreOp::Store;
		//rpDesc.depthAttachment = attach;

		//registry["HDRRenderPass"] = RenderPass::Create(rpDesc);

		rpDesc.colorAttachments.clear();
		attach.format = RenderFormat::R16G16B16A16_FLOAT;
		attach.loadOp = AttachmentLoadOp::Clear;
		attach.storeOp = AttachmentStoreOp::Store;
		rpDesc.colorAttachments.push_back(attach);

		attach.format = RenderFormat::D24_UNORM_S8_UINT;
		attach.loadOp = AttachmentLoadOp::Clear;
		attach.storeOp = AttachmentStoreOp::Store;
		rpDesc.depthAttachment = attach;

		registry["RGBA16FRenderPass"] = RenderPass::Create(rpDesc);
	}
}