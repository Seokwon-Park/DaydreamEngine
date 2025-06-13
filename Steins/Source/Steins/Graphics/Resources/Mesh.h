#pragma once

#include "Steins/Graphics/Resources/Buffer.h"

namespace Steins
{
	class Mesh
	{
		Mesh();

		void Load(const FilePath& _filepath);
	private:
		Shared<VertexBuffer> vertexBuffer;
		Shared<IndexBuffer> indexBuffer;
	};
}