#pragma once

#include "Mesh.h"

namespace Daydream
{
	class Model
	{
	public:
		Model();
		Model(Shared<Mesh> _mesh);
		
		void Load(const String& _path);
		const Array<Shared<Mesh>>& GetMeshes() { return meshes; }
	private:
		Array<Shared<Mesh>> meshes;
		Array<Shared<Material>> materials;
	};
}