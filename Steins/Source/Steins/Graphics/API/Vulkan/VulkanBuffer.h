#pragma once

#include "Steins/Graphics/Resources/Buffer.h"

#include "VulkanRenderDevice.h"

namespace Steins
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(VulkanRenderDevice* _device, UInt32 _bufferSize, UInt32 _stride);
		VulkanVertexBuffer(VulkanRenderDevice* _device, void* _vertices, UInt32 _size, UInt32 _stride);
		virtual ~VulkanVertexBuffer() override;

		virtual void Bind() const;
		virtual void Unbind() const;
		virtual void SetData(void* _data, UInt32 _dataSize) override;
	private:
		VulkanRenderDevice* device;
		VkBuffer uploadBuffer;
		VkDeviceMemory uploadBufferMemory;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(VulkanRenderDevice* _device, UInt32* _indices, UInt32 _indexCount);
		virtual ~VulkanIndexBuffer() override;

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		VulkanRenderDevice* device;
		VkBuffer uploadBuffer;
		VkDeviceMemory uploadBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

	};

	class VulkanConstantBuffer : public ConstantBuffer
	{
	public:
		VulkanConstantBuffer(VulkanRenderDevice* _device, UInt32 _size);
		virtual ~VulkanConstantBuffer() override;

		virtual void Bind(UInt32 _slot, ShaderStage _flags) const override;
		virtual void Update(const void* _data, UInt32 _size);

		virtual void* GetNativeHandle() override { return constantBuffer; }
	private:
		VulkanRenderDevice* device;
		VkBuffer constantBuffer;
		VkDeviceMemory constantBufferMemory;

	};
}

