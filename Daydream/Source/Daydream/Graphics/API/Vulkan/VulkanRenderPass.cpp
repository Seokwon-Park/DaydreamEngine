#include "DaydreamPCH.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"

#include "VulkanUtility.h"

namespace Daydream
{
	VulkanRenderPass::VulkanRenderPass(VulkanRenderDevice* _device, const RenderPassDesc& _desc)
	{
		device = _device;
		desc = _desc;
		std::vector<vk::AttachmentDescription> attachments;
		std::vector<vk::AttachmentReference> colorAttachmentRefs;

		// 컬러 attachment들
		for (size_t i = 0; i < _desc.colorAttachments.size(); ++i)
		{
			vk::AttachmentDescription colorAttachment{};
			colorAttachment.format = GraphicsUtility::Vulkan::ConvertRenderFormatToVkFormat(_desc.colorAttachments[i].format);
			colorAttachment.samples = static_cast<vk::SampleCountFlagBits>(_desc.samples);
			colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
			colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
			colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
			colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
			colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
			colorAttachment.finalLayout =_desc.colorAttachments[i].isSwapchain ? vk::ImageLayout::ePresentSrcKHR : vk::ImageLayout::eShaderReadOnlyOptimal;

			attachments.push_back(colorAttachment);

			vk::AttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = static_cast<uint32_t>(i);
			colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
			colorAttachmentRefs.push_back(colorAttachmentRef);
		}

		// 깊이 attachment
		vk::AttachmentReference depthAttachmentRef{};
		bool hasDepth = (_desc.depthAttachment.format != RenderFormat::UNKNOWN);
		if (hasDepth)
		{
			vk::AttachmentDescription depthAttachment{};
			depthAttachment.format = GraphicsUtility::Vulkan::ConvertRenderFormatToVkFormat(_desc.depthAttachment.format);
			depthAttachment.samples = static_cast<vk::SampleCountFlagBits>(_desc.samples);
			depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
			depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
			depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
			depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
			depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
			depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

			attachments.push_back(depthAttachment);

			depthAttachmentRef.attachment = static_cast<uint32_t>(attachments.size() - 1);
			depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
		}

		vk::SubpassDescription subpass{};
		subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
		subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentRefs.size());
		subpass.pColorAttachments = colorAttachmentRefs.data();
		subpass.pDepthStencilAttachment = hasDepth ? &depthAttachmentRef : nullptr;

		vk::SubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		dependency.srcAccessMask = {};
		dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

		vk::RenderPassCreateInfo renderPassInfo{};
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		renderPass = device->GetDevice().createRenderPassUnique(renderPassInfo);
	}
	VulkanRenderPass::~VulkanRenderPass()
	{
		currentFramebuffer.reset();
	}
	void VulkanRenderPass::Begin(Shared<Framebuffer> _framebuffer)
	{
		DAYDREAM_CORE_ASSERT(device->GetAPI() == RendererAPIType::Vulkan, "Wrong API");
		currentFramebuffer = static_pointer_cast<VulkanFramebuffer>(_framebuffer);

		vk::RenderPassBeginInfo renderPassInfo{};
		renderPassInfo.renderPass = renderPass.get();
		renderPassInfo.framebuffer = currentFramebuffer->GetFramebuffer();
		renderPassInfo.renderArea.offset = vk::Offset2D(0, 0);
		renderPassInfo.renderArea.extent = currentFramebuffer->GetExtent();

		Array<vk::ClearValue> colors;
		for (int i = 0; i < desc.colorAttachments.size(); i++)
		{
			vk::ClearValue vulkanClearColor;
			memcpy(vulkanClearColor.color.float32, clearColor.color, sizeof(clearColor));
			colors.push_back(vulkanClearColor);
		}

		if (currentFramebuffer->HasDepthAttachment())
		{
			vk::ClearValue vulkanClearDepthStencil;
			vulkanClearDepthStencil.depthStencil.depth = 1.0f; // 또는 0.0f
			vulkanClearDepthStencil.depthStencil.stencil = 0;   // 스텐실 값도 함께 초기화
			colors.push_back(vulkanClearDepthStencil);
		}

		renderPassInfo.clearValueCount = colors.size();
		renderPassInfo.pClearValues = colors.data();

		device->GetCommandBuffer().beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
		device->SetCurrentRenderPass(renderPass.get());
		vk::Viewport viewport{};
		//viewport.x = 0.0f;
		//viewport.y = (float)extent.height;
		//viewport.width = (float)extent.width;
		//viewport.height = -(float)extent.height;
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)currentFramebuffer->GetExtent().width;
		viewport.height = (float)currentFramebuffer->GetExtent().height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		device->GetCommandBuffer().setViewport(0, 1, &viewport);

		vk::Rect2D scissor{};
		scissor.offset = vk::Offset2D(0, 0);
		scissor.extent = currentFramebuffer->GetExtent();
		device->GetCommandBuffer().setScissor(0, 1, &scissor);
	}
	void VulkanRenderPass::End()
	{
 		device->GetCommandBuffer().endRenderPass();
	}
	Shared<Framebuffer> VulkanRenderPass::CreateFramebuffer(const FramebufferDesc& _desc)
	{
		return MakeShared<VulkanFramebuffer>(device, this, _desc);
	}
	//void VulkanRenderPass::Begin(Shared<Framebuffer> _framebuffer)
	//{
	//	VkRenderPassBeginInfo renderPassInfo{};
	//	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	//	renderPassInfo.renderPass = renderPass;
	//	renderPassInfo.framebuffer = static_cast<VulkanFramebuffer*>(_framebuffer.get())->GetFramebuffer();
	//	renderPassInfo.renderArea.offset = { 0, 0 };
	//	renderPassInfo.renderArea.extent = { 1080, 720 };

	//	vkCmdBeginRenderPass(device->GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	//}
	//void VulkanRenderPass::End()
	//{
	//	vkCmdEndRenderPass();
	//}
}
