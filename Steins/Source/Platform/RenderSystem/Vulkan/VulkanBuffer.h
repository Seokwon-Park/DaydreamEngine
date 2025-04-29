#pragma once

#include "Steins/Render/Buffer.h"

#include "VulkanGraphicsDevice.h"

namespace Steins
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(VulkanGraphicsDevice* _device, void* _vertices, UInt32 _size, const BufferLayout& _layout);
		virtual ~VulkanVertexBuffer() override;

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		VulkanGraphicsDevice* device;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(VulkanGraphicsDevice* _device, UInt32* _indices, UInt32 _indexCount);
		virtual ~VulkanIndexBuffer() override;

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		VulkanGraphicsDevice* device;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

	};
}

