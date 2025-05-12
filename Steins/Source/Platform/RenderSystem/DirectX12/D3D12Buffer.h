#pragma once

#include "Steins/Render/Buffer.h"
#include "D3D12GraphicsDevice.h"

namespace Steins
{
	class D3D12VertexBuffer : public VertexBuffer
	{
	public:
		D3D12VertexBuffer(D3D12GraphicsDevice* _device, void* _vertices, UInt32 _size, const BufferLayout& _layout);

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		D3D12GraphicsDevice* device;
		ComPtr<ID3D12Resource> vertexBuffer;
		ComPtr<ID3D12Resource> uploadBuffer;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	};

	class D3D12IndexBuffer : public IndexBuffer
	{
	public:
		D3D12IndexBuffer(D3D12GraphicsDevice* _device, UInt32* _indices, UInt32 _indexCount);

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private :
		D3D12GraphicsDevice* device;
		ComPtr<ID3D12Resource> indexBuffer;
		ComPtr<ID3D12Resource> uploadBuffer;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
	};

	class D3D12ConstantBuffer : public ConstantBuffer
	{
	public:
		D3D12ConstantBuffer(D3D12GraphicsDevice* _device, UInt32 _size);

		virtual void Bind(UInt32 _slot, ShaderStage _flags) const override;

		virtual void Update(const void* _data, UInt32 _size) override;

		virtual void* GetNativeHandle() override { return constantBuffer.Get(); }

	private:
		D3D12GraphicsDevice* device;
		ComPtr<ID3D12Resource> constantBuffer;
		D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferView;
	};
}