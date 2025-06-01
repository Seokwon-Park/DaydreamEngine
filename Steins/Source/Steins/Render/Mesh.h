#pragma once

#include "Buffer.h"

namespace Steins
{
	class Mesh
	{
		Mesh();
	private:
		Shared<VertexBuffer> vertexBuffer;
		Shared<VertexBuffer> indexBuffer;
	};
}