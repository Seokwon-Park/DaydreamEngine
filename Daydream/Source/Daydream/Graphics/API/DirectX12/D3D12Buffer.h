#pragma once

#include "Daydream/Graphics/Resources/Buffer.h"

#include "D3D12RenderDevice.h"

namespace Daydream
{
	class D3D12VertexBuffer : public VertexBuffer
	{
	public:
		D3D12VertexBuffer(D3D12RenderDevice* _device, BufferUsage _usage, UInt32 _size, UInt32 _stride);
		virtual ~D3D12VertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void* GetNativeHandle() const override { return (void*)&vertexBufferView; };
		virtual void SetData(const void * _data, UInt32 _dataSize) override;
		
		ID3D12Resource* GetDX12Buffer() { return vertexBuffer.Get(); }
	private:
		UInt32 bufferSize;
		D3D12RenderDevice* device;
		ComPtr<ID3D12Resource> vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		void* mappedData;
	};

	class D3D12IndexBuffer : public IndexBuffer
	{
	public:
		D3D12IndexBuffer(D3D12RenderDevice* _device, UInt32 _indexCount);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void* GetNativeHandle() const override { return (void*)&indexBufferView; };
		ID3D12Resource* GetDX12Buffer() { return indexBuffer.Get(); }
	private :
		D3D12RenderDevice* device;
		ComPtr<ID3D12Resource> indexBuffer;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
	};

	class D3D12ConstantBuffer : public ConstantBuffer
	{
	public:
		D3D12ConstantBuffer(D3D12RenderDevice* _device, UInt32 _size);
		virtual ~D3D12ConstantBuffer();


		//virtual void Bind(UInt32 _slot) const override;

		virtual void Update(const void* _data, UInt32 _size) override;

		virtual void* GetNativeHandle() override { return constantBuffer.Get(); }
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return constantBuffer->GetGPUVirtualAddress(); }

	private:
		D3D12RenderDevice* device;
		ComPtr<ID3D12Resource> constantBuffer;
		D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferView;
		void* mappedData;

	};
}