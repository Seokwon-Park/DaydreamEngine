#pragma once

#include "Steins/Render/Buffer.h"
#include "D3D11GraphicsDevice.h"

namespace Steins
{
	class D3D11VertexBuffer : public VertexBuffer
	{
	public:
		D3D11VertexBuffer(D3D11GraphicsDevice* _device, void* _vertices, UInt32 _size, const BufferLayout& _layout);
		virtual ~D3D11VertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		D3D11GraphicsDevice* device;
		ComPtr<ID3D11Buffer> vertexBuffer;
	};

	class D3D11IndexBuffer : public IndexBuffer
	{
	public:
		D3D11IndexBuffer(D3D11GraphicsDevice* _device, UInt32* _indices, UInt32 _indexCount);
		virtual ~D3D11IndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		D3D11GraphicsDevice* device;
		ComPtr<ID3D11Buffer> indexBuffer;
	};


	//class D3D11ConstantBuffer : public ConstantBuffer
	//{
	//public:
	//	D3D11ConstantBuffer(D3D11GraphicsDevice* _device, UInt32* _indices, UInt32 _indexCount);
	//	virtual ~D3D11ConstantBuffer();

	//	virtual void Bind() const;

	//private:
	//	D3D11GraphicsDevice* device;
	//	ComPtr<ID3D11Buffer> indexBuffer;
	//};
}

