#include "DaydreamPCH.h"
#include "Model.h"

#include "Daydream/Graphics/Utility/ModelLoader.h"
#include "Daydream/Graphics/Resources/Buffer.h"
#include "Daydream/Graphics/Resources/Mesh.h"
#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	Model::Model()
	{
	}
	Model::Model(Shared<Mesh> _mesh)
	{
		meshes.push_back(_mesh);
	}
	void Model::Load(const String& _path)
	{
		meshes.clear();
		
		FilePath filepath(_path);

		Array<MeshData> meshDatas = ModelLoader::LoadFromFile(filepath);

		Vector3 vmin(1000, 1000, 1000);
		Vector3 vmax(-1000, -1000, -1000);
		for (auto meshData : meshDatas)
		{
			// Normalize vertices

			for (auto& v : meshData.vertices) {
				vmin.x = std::min(vmin.x, v.position.x);
				vmin.y = std::min(vmin.y, v.position.y);
				vmin.z = std::min(vmin.z, v.position.z);
				vmax.x = std::max(vmax.x, v.position.x);
				vmax.y = std::max(vmax.y, v.position.y);
				vmax.z = std::max(vmax.z, v.position.z);
			}
		}

		for (auto meshData : meshDatas)
		{
			float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
			float dl = std::max(std::max(dx, dy), dz);
			float cx = (vmax.x + vmin.x) * 0.5f, cy = (vmax.y + vmin.y) * 0.5f,
				cz = (vmax.z + vmin.z) * 0.5f;

			for (auto& v : meshData.vertices)
			{
				v.position.x = (v.position.x - cx) / dl;
				v.position.y = (v.position.y - cy) / dl;
				v.position.z = (v.position.z - cz) / dl;
			}

			Shared<Mesh> mesh = Mesh::Create(meshData);
			meshes.push_back(mesh);
		}
	}
}
