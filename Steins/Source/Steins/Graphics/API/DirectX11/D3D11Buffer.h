#pragma once

#include "Steins/Graphics/Resources/Buffer.h"
#include "D3D11RenderDevice.h"

namespace Steins
{
	class D3D11VertexBuffer : public VertexBuffer
	{
	public:
		D3D11VertexBuffer(D3D11RenderDevice* _device, UInt32 _bufferSize, UInt32 _stride);
		D3D11VertexBuffer(D3D11RenderDevice* _device, void* _vertices, UInt32 _size, UInt32 _stride);
		virtual ~D3D11VertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual void SetData(void* _data, UInt32 _dataSize) override;
	private:
		UInt32 stride;
		D3D11RenderDevice* device;
		ComPtr<ID3D11Buffer> vertexBuffer;
	};

	class D3D11IndexBuffer : public IndexBuffer
	{
	public:
		D3D11IndexBuffer(D3D11RenderDevice* _device, UInt32* _indices, UInt32 _indexCount);
		virtual ~D3D11IndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		D3D11RenderDevice* device;
		ComPtr<ID3D11Buffer> indexBuffer;
	};


	class D3D11ConstantBuffer : public ConstantBuffer
	{
	public:
		D3D11ConstantBuffer(D3D11RenderDevice* _device, UInt32 _size);
		virtual ~D3D11ConstantBuffer();

		virtual void Bind(UInt32 _slot) const override;

		virtual void Update(const void* _data, UInt32 _size) override;

		virtual void* GetNativeHandle() override { return constantBuffer.Get(); }

	private:
		D3D11RenderDevice* device;
		ComPtr<ID3D11Buffer> constantBuffer;
	};
}

