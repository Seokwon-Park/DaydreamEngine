#pragma once

#include "Steins/Graphics/Resources/Buffer.h"
#include "D3D12RenderDevice.h"

namespace Steins
{
	class D3D12VertexBuffer : public VertexBuffer
	{
	public:
		D3D12VertexBuffer(D3D12RenderDevice* _device, UInt32 _bufferSize, UInt32 _stride);
		D3D12VertexBuffer(D3D12RenderDevice* _device, void* _vertices, UInt32 _size, UInt32 _stride);

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetData(void* _data, UInt32 _dataSize) override;
	private:
		UInt32 stride;
		D3D12RenderDevice* device;
		ComPtr<ID3D12Resource> vertexBuffer;
		ComPtr<ID3D12Resource> uploadBuffer;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	};

	class D3D12IndexBuffer : public IndexBuffer
	{
	public:
		D3D12IndexBuffer(D3D12RenderDevice* _device, UInt32* _indices, UInt32 _indexCount);

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private :
		D3D12RenderDevice* device;
		ComPtr<ID3D12Resource> indexBuffer;
		ComPtr<ID3D12Resource> uploadBuffer;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
	};

	class D3D12ConstantBuffer : public ConstantBuffer
	{
	public:
		D3D12ConstantBuffer(D3D12RenderDevice* _device, UInt32 _size);

		//virtual void Bind(UInt32 _slot) const override;

		virtual void Update(const void* _data, UInt32 _size) override;

		virtual void* GetNativeHandle() override { return constantBuffer.Get(); }
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return constantBuffer->GetGPUVirtualAddress(); }

	private:
		D3D12RenderDevice* device;
		ComPtr<ID3D12Resource> constantBuffer;
		D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferView;
	};
}