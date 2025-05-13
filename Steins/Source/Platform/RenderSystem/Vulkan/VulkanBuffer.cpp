#include "SteinsPCH.h"
#include "VulkanBuffer.h"

namespace Steins
{
	VulkanVertexBuffer::VulkanVertexBuffer(VulkanGraphicsDevice* _device, void* _vertices, UInt32 _size, const BufferLayout& _layout)
	{
		device = _device;
		layout = _layout;

		device->CreateBuffer(_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uploadBuffer, uploadBufferMemory);


		void* data;
		vkMapMemory(device->GetDevice(), uploadBufferMemory, 0, _size, 0, &data);
		memcpy(data, _vertices, _size);
		vkUnmapMemory(device->GetDevice(), uploadBufferMemory);

		device->CreateBuffer(_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer, vertexBufferMemory);

		device->CopyBuffer(uploadBuffer, vertexBuffer, _size);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		vkDestroyBuffer(device->GetDevice(), uploadBuffer, nullptr);
		vkFreeMemory(device->GetDevice(), uploadBufferMemory, nullptr);

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

		UInt32 bufferSize = sizeof(UInt32) * _indexCount;

		device->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uploadBuffer, uploadBufferMemory);

		void* data;
		vkMapMemory(device->GetDevice(), uploadBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, _indices, bufferSize);
		vkUnmapMemory(device->GetDevice(), uploadBufferMemory);

		device->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			indexBuffer, indexBufferMemory);

		device->CopyBuffer(uploadBuffer, indexBuffer, bufferSize);
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		vkDestroyBuffer(device->GetDevice(), uploadBuffer, nullptr);
		vkFreeMemory(device->GetDevice(), uploadBufferMemory, nullptr);

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

	VulkanConstantBuffer::VulkanConstantBuffer(VulkanGraphicsDevice* _device, UInt32 _size)
	{
		device = _device;
		device->CreateBuffer(_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, constantBuffer, constantBufferMemory);
		vkMapMemory(device->GetDevice(), constantBufferMemory, 0, _size, 0, &data);

	}

	VulkanConstantBuffer::~VulkanConstantBuffer()
	{
		vkDestroyBuffer(device->GetDevice(), constantBuffer, nullptr);
		vkFreeMemory(device->GetDevice(), constantBufferMemory, nullptr);
	}

	void VulkanConstantBuffer::Bind(UInt32 _slot, ShaderStage _flags) const
	{
		
	}

	void VulkanConstantBuffer::Update(const void* _data, UInt32 _size)
	{
		memcpy(data, _data, _size);
	}

}
