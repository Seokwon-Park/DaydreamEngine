#include "DaydreamPCH.h"
#include "RenderPassRegistry.h"

namespace Daydream
{
	//void RenderPassRegistry::CreateBuiltinResources()
	//{
	//	RenderPassDesc rpDesc;

	//	RenderPassAttachmentDesc attach{};
	//	//attach.format = RenderFormat::R8G8B8A8_UNORM;
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.colorAttachments.push_back(attach);

	//	//attach.format = RenderFormat::R24G8_TYPELESS;
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.depthAttachment = attach;

	//	registry["StandardRenderPass"] = RenderPass::Create(rpDesc);

	//	// === G-Buffer RenderPass ===
	//	rpDesc.colorAttachments.clear();

	//	//attach.format = RenderFormat::R32_UINT;
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.colorAttachments.push_back(attach);

	//	//attach.format = RenderFormat::UNKNOWN;
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.depthAttachment = attach;

	//	registry["MaskRenderPass"] = RenderPass::Create(rpDesc);

	//	// === Depth RenderPass ===
	//	rpDesc.colorAttachments.clear();

	//	//attach.format = RenderFormat::R24G8_TYPELESS;
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.depthAttachment = attach;

	//	registry["DepthRenderPass"] = RenderPass::Create(rpDesc);

	//	// === G-Buffer RenderPass ===
	//	rpDesc.colorAttachments.clear();

	//	// Attachment 0: Position (View Space)
	//	//attach.format = RenderFormat::R16G16B16A16_FLOAT;
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.colorAttachments.push_back(attach);

	//	// Attachment 1: Normal (View Space)
	//	//attach.format = RenderFormat::R16G16B16A16_FLOAT; // 또는 R10G10B10A2_UNORM 등으로 압축
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.colorAttachments.push_back(attach);

	//	// Attachment 2: Albedo (Diffuse Color)
	//	//attach.format = RenderFormat::R8G8B8A8_UNORM;
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.colorAttachments.push_back(attach);

	//	// Attachment 3: Metallic / Roughness / AO / Emissive
	//	//attach.format = RenderFormat::R8G8B8A8_UNORM; // 채널별로 R=Metallic, G=Roughness, ...
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.colorAttachments.push_back(attach);

	//	// Attachment 4: EntityID
	//	//attach.format = RenderFormat::R32_UINT; 
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	//attach.type = AttachmentType::EntityHandle;
	//	rpDesc.colorAttachments.push_back(attach);

	//	// Depth Attachment
	//	//attach.format = RenderFormat::R24G8_TYPELESS;
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	//attach.type = AttachmentType::None;
	//	rpDesc.depthAttachment = attach;

	//	registry["GBufferRenderPass"] = RenderPass::Create(rpDesc);


	//	// Mipmap 생성을 위한 RenderPass (Depth Buffer 없음)
	//	rpDesc.colorAttachments.clear();
	//	//attach.format = RenderFormat::R16G16B16A16_FLOAT; // 밉맵을 생성할 텍스처 포맷에 맞춰주세요.
	//	attach.loadOp = AttachmentLoadOp::Clear; // 이 전 내용이 필요 없으므로 DontCare나 Clear
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.colorAttachments.push_back(attach);

	//	rpDesc.depthAttachment = {};
	//	//attach.format = RenderFormat::UNKNOWN;
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.depthAttachment = attach;

	//	registry["MipmapRenderPass"] = RenderPass::Create(rpDesc);
	//	//rpDesc.colorAttachments.clear();
	//	//attach.format = RenderFormat::R32G32B32A32_FLOAT;
	//	//attach.loadOp = AttachmentLoadOp::Clear;
	//	//attach.storeOp = AttachmentStoreOp::Store;
	//	//rpDesc.colorAttachments.push_back(attach);

	//	//attach.format = RenderFormat::D24_UNORM_S8_UINT;
	//	//attach.loadOp = AttachmentLoadOp::Clear;
	//	//attach.storeOp = AttachmentStoreOp::Store;
	//	//rpDesc.depthAttachment = attach;

	//	//registry["HDRRenderPass"] = RenderPass::Create(rpDesc);

	//	rpDesc.colorAttachments.clear();
	//	//attach.format = RenderFormat::R16G16B16A16_FLOAT;
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.colorAttachments.push_back(attach);

	//	//attach.format = RenderFormat::R24G8_TYPELESS;
	//	attach.loadOp = AttachmentLoadOp::Clear;
	//	attach.storeOp = AttachmentStoreOp::Store;
	//	rpDesc.depthAttachment = attach;

	//	registry["RGBA16FRenderPass"] = RenderPass::Create(rpDesc);
	//}

}