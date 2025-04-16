#pragma once

#include "Steins/Render/Buffer.h"

namespace Steins
{
	class D3D12VertexBuffer : public VertexBuffer
	{
	public:
	private:
		ComPtr<ID3D12Resource> vertexBuffer;
	};
}