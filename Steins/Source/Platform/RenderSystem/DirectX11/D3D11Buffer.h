#pragma once

#include "Steins/Render/Buffer.h"

namespace Steins
{
	class D3D11VertexBuffer : public VertexBuffer
	{
	public:
		D3D11VertexBuffer(Float32* _vertices, UInt32 _size);
		virtual ~D3D11VertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		ComPtr<ID3D11Buffer> vertexBuffer;
			
	};

	class D3D11IndexBuffer : public IndexBuffer
	{
	public:
		D3D11IndexBuffer(UInt32* _indices, UInt32 _IndexCount);
		virtual ~D3D11IndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual UInt32 GetCount() const { return indexCount; }
	private:
		ComPtr<ID3D11Buffer> indexBuffer;
		UInt32 indexCount;
	};
}

