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
		uint32_t rendererID;
	private:
		ComPtr<ID3D11Buffer> vertexBuffer;
			
	};
}

