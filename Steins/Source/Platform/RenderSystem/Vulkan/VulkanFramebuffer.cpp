#include "SteinsPCH.h"
#include "VulkanFramebuffer.h"
#include "VulkanSwapchain.h"


namespace Steins
{
    VulkanFramebuffer::VulkanFramebuffer(VulkanGraphicsDevice* _device, const FramebufferSpecification& _spec)
    {
        device = _device;
    }
    VulkanFramebuffer::VulkanFramebuffer(VulkanGraphicsDevice* _device, VulkanSwapChain* _swapChain)
    {
        device = _device;

        UInt32 swapChainImageCount = 0;
        vkGetSwapchainImagesKHR(device->GetLogicalDevice(), _swapChain->GetVKSwapChain(), &swapChainImageCount, nullptr);
        colorImages.resize(swapChainImageCount);
        vkGetSwapchainImagesKHR(device->GetLogicalDevice(), _swapChain->GetVKSwapChain(), &swapChainImageCount, colorImages.data());

        colorImageViews.resize(swapChainImageCount);

        for (UInt32 i = 0; i < swapChainImageCount; i++)
        {
            VkImageViewCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            info.image = colorImages[i];
            info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            info.format = _swapChain->GetFormat();
            info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            info.subresourceRange.baseMipLevel = 0;
            info.subresourceRange.levelCount = 1;
            info.subresourceRange.baseArrayLayer = 0;
            info.subresourceRange.layerCount = 1;

            VkResult result = vkCreateImageView(device->GetLogicalDevice(), &info, nullptr, &colorImageViews[i]);
            STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create image view!");
        }

        for (size_t i = 0; i < colorImageViews.size(); i++) {
            VkImageView attachments[] = {
                colorImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = device->GetRenderPass();
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = _swapChain->GetExtent().width;
            framebufferInfo.height = _swapChain->GetExtent().height;
            framebufferInfo.layers = 1;

            VkResult result = vkCreateFramebuffer(device->GetLogicalDevice(), &framebufferInfo, nullptr, &framebuffers[i]);
            STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create framebuffer!");
        }

    }
    VulkanFramebuffer::~VulkanFramebuffer()
    {
        for (auto imageView : colorImageViews)
        {
            vkDestroyImageView(device->GetLogicalDevice(), imageView, nullptr);
        }
    }
    void VulkanFramebuffer::Bind() const
    {
    }
}