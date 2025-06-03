#pragma once

#include "Steins/Render/Buffer.h"

#include "VulkanGraphicsDevice.h"

namespace Steins
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(VulkanGraphicsDevice* _device, UInt32 _bufferSize, UInt32 _stride);
		VulkanVertexBuffer(VulkanGraphicsDevice* _device, void* _vertices, UInt32 _size, UInt32 _stride);
		virtual ~VulkanVertexBuffer() override;

		virtual void Bind() const;
		virtual void Unbind() const;
		virtual void SetData(void* _data, UInt32 _dataSize) override;
	private:
		VulkanGraphicsDevice* device;
		VkBuffer uploadBuffer;
		VkDeviceMemory uploadBufferMemory;
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
		VkBuffer uploadBuffer;
		VkDeviceMemory uploadBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

	};

	class VulkanConstantBuffer : public ConstantBuffer
	{
	public:
		VulkanConstantBuffer(VulkanGraphicsDevice* _device, UInt32 _size);
		virtual ~VulkanConstantBuffer() override;

		virtual void Bind(UInt32 _slot, ShaderStage _flags) const override;
		virtual void Update(const void* _data, UInt32 _size);

		virtual void* GetNativeHandle() override { return constantBuffer; }
	private:
		VulkanGraphicsDevice* device;
		VkBuffer constantBuffer;
		VkDeviceMemory constantBufferMemory;

	};
}

