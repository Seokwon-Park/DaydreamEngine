#include "SteinsPCH.h"
#include "VulkanBuffer.h"

namespace Steins
{
	VulkanVertexBuffer::VulkanVertexBuffer(VulkanGraphicsDevice* _device, void* _vertices, UInt32 _size)
	{
		device = _device;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _size;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkResult result = vkCreateBuffer(device->GetDevice(), &bufferInfo, nullptr, &vertexBuffer);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create vertex buffer!");
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		vkDestroyBuffer(device->GetDevice(), vertexBuffer, nullptr);
	}

	void VulkanVertexBuffer::Bind() const
	{
	}

	void VulkanVertexBuffer::Unbind() const
	{
	}

	VulkanIndexBuffer::VulkanIndexBuffer(VulkanGraphicsDevice* _device, UInt32* _indices, UInt32 _indexCount)
	{
		device = _device;
		indexCount = _indexCount;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		//bufferInfo.size = _size;
		bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkResult result = vkCreateBuffer(device->GetDevice(), &bufferInfo, nullptr, &indexBuffer);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create vertex buffer!");
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}

	void VulkanIndexBuffer::Bind() const
	{
	}

	void VulkanIndexBuffer::Unbind() const
	{
	}

}
