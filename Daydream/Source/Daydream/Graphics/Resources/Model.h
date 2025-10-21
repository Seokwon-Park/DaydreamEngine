#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Daydream/Core/Resource.h"

namespace Daydream
{
	struct ModelData
	{
		Array<MeshData> meshes;
		Array<MaterialData> materials;
	};

	class Model : public Resource
	{
	public:
		Model();
		Model(Shared<Mesh> _mesh);
		~Model();
		
		void Load(const String& _path);
		const Array<Shared<Mesh>>& GetMeshes() { return meshes; }
		const Array<Shared<Material>>& GetMaterials() { return materials; }
		
		static Shared<Model> Create();
		static Shared<Model> Create(Shared<Mesh> _mesh);
	private:
		Array<Shared<Mesh>> meshes;
		Array<Shared<Material>> materials;
	};
}