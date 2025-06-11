#pragma once

#include "Buffer.h"

namespace Steins
{
	class Mesh
	{
		Mesh();

		void Load(const FilePath& _filepath);

		void DrawMesh();
	private:
		Shared<VertexBuffer> vertexBuffer;
		Shared<VertexBuffer> indexBuffer;
	};
}