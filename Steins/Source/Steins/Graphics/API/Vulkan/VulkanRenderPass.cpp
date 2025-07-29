#include "SteinsPCH.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"

#include "Steins/Graphics/Utility/GraphicsUtil.h"

namespace Steins
{
	VulkanRenderPass::VulkanRenderPass(VulkanRenderDevice* _device, const RenderPassDesc& _desc)
	{
		device = _device;
		desc = _desc;
		std::vector<VkAttachmentDescription> attachments;
		std::vector<VkAttachmentReference> colorAttachmentRefs;

		// 컬러 attachment들
		for (size_t i = 0; i < _desc.colorAttachments.size(); ++i)
		{
			VkAttachmentDescription colorAttachment{};
			colorAttachment.format = GraphicsUtil::ConvertRenderFormatToVkFormat(_desc.colorAttachments[i].format);
			colorAttachment.samples = static_cast<VkSampleCountFlagBits>(_desc.samples);
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = _desc.isSwapchain? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			attachments.push_back(colorAttachment);

			VkAttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = static_cast<uint32_t>(i);
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			colorAttachmentRefs.push_back(colorAttachmentRef);
		}

		// 깊이 attachment
		VkAttachmentReference depthAttachmentRef{};
		bool hasDepth = (_desc.depthAttachment.format != RenderFormat::UNKNOWN);
		if (hasDepth)
		{
			VkAttachmentDescription depthAttachment{};
			depthAttachment.format = GraphicsUtil::ConvertRenderFormatToVkFormat(_desc.depthAttachment.format);
			depthAttachment.samples = static_cast<VkSampleCountFlagBits>(_desc.samples);
			depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			attachments.push_back(depthAttachment);

			depthAttachmentRef.attachment = static_cast<uint32_t>(attachments.size() - 1);
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentRefs.size());
		subpass.pColorAttachments = colorAttachmentRefs.data();
		subpass.pDepthStencilAttachment = hasDepth ? &depthAttachmentRef : nullptr;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		VkResult result = vkCreateRenderPass(device->GetDevice(), &renderPassInfo, nullptr, &renderPass);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create renderpass!");
	}
	VulkanRenderPass::~VulkanRenderPass()
	{
		if (renderPass != VK_NULL_HANDLE) vkDestroyRenderPass(device->GetDevice(), renderPass, nullptr);
	}
	void VulkanRenderPass::Begin(Shared<Framebuffer> _framebuffer)
	{
		STEINS_CORE_ASSERT(device->GetAPI() == RendererAPIType::Vulkan, "Wrong API");
		currentFramebuffer = static_pointer_cast<VulkanFramebuffer>(_framebuffer);

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = currentFramebuffer->GetFramebuffer();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = currentFramebuffer->GetExtent();

		Array<VkClearValue> colors;
		for (int i = 0; i < desc.colorAttachments.size(); i++)
		{
			VkClearValue vulkanClearColor;
			memcpy(vulkanClearColor.color.float32, clearColor.color, sizeof(clearColor));
			colors.push_back(vulkanClearColor);
		}

		if (currentFramebuffer->HasDepthAttachment())
		{
			VkClearValue vulkanClearDepthStencil;
			vulkanClearDepthStencil.depthStencil.depth = 1.0f; // 또는 0.0f
			vulkanClearDepthStencil.depthStencil.stencil = 0;   // 스텐실 값도 함께 초기화
			colors.push_back(vulkanClearDepthStencil);
		}

		renderPassInfo.clearValueCount = colors.size();
		renderPassInfo.pClearValues = colors.data();

		vkCmdBeginRenderPass(device->GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		device->SetCurrentRenderPass(renderPass);
		VkViewport viewport{};
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
		vkCmdSetViewport(device->GetCommandBuffer(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = currentFramebuffer->GetExtent();
		vkCmdSetScissor(device->GetCommandBuffer(), 0, 1, &scissor);
	}
	void VulkanRenderPass::End()
	{
		vkCmdEndRenderPass(device->GetCommandBuffer());
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
