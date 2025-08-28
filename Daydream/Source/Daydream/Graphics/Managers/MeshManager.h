#pragma once

#include "Daydream/Graphics/Resources/Mesh.h"

namespace Daydream
{
	class MeshManager
	{
	public:
		MeshManager();
	private:
		HashMap<String, Shared<Mesh>> meshCache;
	};
}