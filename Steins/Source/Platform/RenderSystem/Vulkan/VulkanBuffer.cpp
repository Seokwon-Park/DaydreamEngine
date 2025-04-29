#include "SteinsPCH.h"
#include "VulkanBuffer.h"

namespace Steins
{
	VulkanVertexBuffer::VulkanVertexBuffer(VulkanGraphicsDevice* _device, void* _vertices, UInt32 _size, const BufferLayout& _layout)
	{
		device = _device;
		layout = _layout;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _size;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkResult result = vkCreateBuffer(device->GetDevice(), &bufferInfo, nullptr, &vertexBuffer);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create vertex buffer!");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device->GetDevice(), vertexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}
		vkBindBufferMemory(device->GetDevice(), vertexBuffer, vertexBufferMemory, 0);
		void* data;
		vkMapMemory(device->GetDevice(), vertexBufferMemory, 0, bufferInfo.size, 0, &data);
		memcpy(data, _vertices, (size_t)bufferInfo.size);
		vkUnmapMemory(device->GetDevice(), vertexBufferMemory);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		vkDestroyBuffer(device->GetDevice(), vertexBuffer, nullptr);
		vkFreeMemory(device->GetDevice(), vertexBufferMemory, nullptr);
	}

	void VulkanVertexBuffer::Bind() const
	{
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(device->GetCommandBuffer(), slot, 1, &vertexBuffer, &offset);
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
		bufferInfo.size = sizeof(UInt32)*indexCount;
		bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkResult result = vkCreateBuffer(device->GetDevice(), &bufferInfo, nullptr, &indexBuffer);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create vertex buffer!");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device->GetDevice(), indexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &indexBufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}
		vkBindBufferMemory(device->GetDevice(), indexBuffer, indexBufferMemory, 0);
		void* data;
		vkMapMemory(device->GetDevice(), indexBufferMemory, 0, bufferInfo.size, 0, &data);
		memcpy(data, _indices, (size_t)bufferInfo.size);
		vkUnmapMemory(device->GetDevice(), indexBufferMemory);
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		vkDestroyBuffer(device->GetDevice(), indexBuffer, nullptr);
		vkFreeMemory(device->GetDevice(), indexBufferMemory, nullptr);
	}

	void VulkanIndexBuffer::Bind() const
	{
		vkCmdBindIndexBuffer(device->GetCommandBuffer(), indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanIndexBuffer::Unbind() const
	{
	}

}
