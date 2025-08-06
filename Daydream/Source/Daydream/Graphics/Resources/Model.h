#pragma once

#include "Mesh.h"

namespace Daydream
{
	class Model
	{
	public:
		Model();
		
		void Load(const String& _path);
		Array<Shared<Mesh>>& GetMeshes() { return meshes; }

		static void CreateBasicModels();
		static Shared<Mesh> Plane;
		static Shared<Mesh> Cube;
		static Shared<Mesh> Sphere;
		static Shared<Mesh> Cone;
	private:
		Array<Shared<Mesh>> meshes;
	};
}