#include "SteinsPCH.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"

namespace Steins
{
	VulkanRenderPass::VulkanRenderPass(VulkanGraphicsDevice* _device)
	{
		device = _device;

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = VK_FORMAT_R8G8B8A8_UNORM;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		VkResult result = vkCreateRenderPass(_device->GetDevice(), &renderPassInfo, nullptr, &renderPass);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create renderpass!");
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
