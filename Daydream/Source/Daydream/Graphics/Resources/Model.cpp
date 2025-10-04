#include "DaydreamPCH.h"
#include "Model.h"

#include "Daydream/Core/ResourceManager.h"

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

		Path filepath(_path);

		ModelData modelData = ModelLoader::LoadFromFile(filepath);

		Vector3 vmin(1000, 1000, 1000);
		Vector3 vmax(-1000, -1000, -1000);
		for (auto meshData : modelData.meshes)
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

		for (auto meshData : modelData.meshes)
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

		for (UInt32 i = 0; i < modelData.materials.size(); i++)
		{
			Shared<Material> newMat = Material::Create(ResourceManager::GetResource<PipelineState>("ForwardPSO"));
			auto diffuse = ResourceManager::GetResource<Texture2D>("Default");
			auto normal = ResourceManager::GetResource<Texture2D>("DefaultNormal");
			auto roughness = ResourceManager::GetResource<Texture2D>("DefaultRoughness");
			auto metallic = ResourceManager::GetResource<Texture2D>("DefaultMetallic");
			auto ao = ResourceManager::GetResource<Texture2D>("DefaultAO");
			newMat->SetTexture2D("AlbedoTexture", diffuse);
			newMat->SetTexture2D("NormalTexture", normal);
			newMat->SetTexture2D("RoughnessTexture", roughness);
			newMat->SetTexture2D("MetallicTexture", metallic);
			newMat->SetTexture2D("AOTexture", ao);
			if (!modelData.materials[i].albedoMapPath.empty())
				newMat->SetTexture2D("AlbedoTexture", ResourceManager::GetResource<Texture2D>(modelData.materials[i].albedoMapPath));
			if (!modelData.materials[i].normalMapPath.empty())
				newMat->SetTexture2D("NormalTexture", ResourceManager::GetResource<Texture2D>(modelData.materials[i].normalMapPath));
			if (!modelData.materials[i].roughnessMapPath.empty())
				newMat->SetTexture2D("RoughnessTexture", ResourceManager::GetResource<Texture2D>(modelData.materials[i].roughnessMapPath));
			if (!modelData.materials[i].metallicMapPath.empty())
				newMat->SetTexture2D("MetallicTexture", ResourceManager::GetResource<Texture2D>(modelData.materials[i].metallicMapPath));
			if (!modelData.materials[i].AOMapPath.empty())
				newMat->SetTexture2D("AOTexture", ResourceManager::GetResource<Texture2D>(modelData.materials[i].AOMapPath));
			materials.push_back(newMat);
		}
	}
	Shared<Model> Model::Create()
	{
		return MakeShared<Model>();
	}

	Shared<Model> Model::Create(Shared<Mesh> _mesh)
	{
		return MakeShared<Model>(_mesh);
	}
}
