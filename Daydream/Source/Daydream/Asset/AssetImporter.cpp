#include "DaydreamPCH.h"
#include "AssetImporter.h"
#include "AssetDefaults.h"
#include "Daydream/Graphics/Utility/ImageLoader.h"
#include "Daydream/Graphics/Utility/ModelLoader.h"
#include "Daydream/Graphics/Manager/ResourceManager.h"
#include "Daydream/Graphics/Resources/PipelineState.h"
#include "yaml-cpp/yaml.h"

namespace Daydream
{
	Shared<Texture2D> AssetImporter::LoadTexture2D(const AssetMetadata& _metaData)
	{
		Path texturePath = _metaData.filePath;
		String pathString = texturePath.make_preferred().string();
		String extension = texturePath.extension().string();

		bool isSRGB = true;
		if (pathString.find("_n.") != std::string::npos ||
			pathString.find("_N.") != std::string::npos ||
			pathString.find("_normal.") != std::string::npos ||
			pathString.find("_Normal.") != std::string::npos ||
			pathString.find("metal") != std::string::npos ||    // metallic, metalness 등
			pathString.find("rough") != std::string::npos ||    // roughness
			pathString.find("ao.") != std::string::npos ||      // ambient occlusion
			pathString.find("height.") != std::string::npos)    // height map
		{
			isSRGB = false;
		}

		ImageData data = ImageLoader::LoadImageFile(pathString);
		TextureDesc desc{};
		desc.bindFlags = RenderBindFlags::ShaderResource;
		desc.width = data.width;
		desc.height = data.height;
		if (extension == ".hdr")
		{
			desc.format = RenderFormat::R32G32B32A32_FLOAT;
		}
		else
		{
			// 8비트 텍스처 (sRGB 또는 Linear)
			desc.format = isSRGB ? RenderFormat::R8G8B8A8_UNORM_SRGB : RenderFormat::R8G8B8A8_UNORM;
		}
		desc.type = TextureType::Texture2D;
		Shared<Texture2D> newTexture = Texture2D::Create(data.GetRawDataPtr(), desc);

		return newTexture;
	}

	Shared<Model> AssetImporter::LoadModel(const AssetMetadata& _metaData)
	{
		Path modelPath = _metaData.filePath;

		String pathString = modelPath.generic_string();
		String extension = modelPath.extension().string();
		String metaFilePathString = pathString + ".ddmeta";

		Shared<ModelData> modelData = ModelLoader::LoadFromFile(modelPath);
		Shared<Model> newModel = Model::Create(modelData);

		YAML::Node metaNode = YAML::LoadFile(metaFilePathString);
		bool isGeneratingNewMeta = _metaData.subAssets.empty();

		if (!metaNode["SubAssets"])
		{
			metaNode["SubAssets"] = YAML::Node(YAML::NodeType::Map);
		}

		//for normalize
		//Vector3 vmin(1000, 1000, 1000);
		//Vector3 vmax(-1000, -1000, -1000);
		//for (auto meshData : modelData->meshes)
		//{
		//	// Normalize vertices
		//	for (auto& v : meshData.vertices) {
		//		vmin.x = std::min(vmin.x, v.position.x);
		//		vmin.y = std::min(vmin.y, v.position.y);
		//		vmin.z = std::min(vmin.z, v.position.z);
		//		vmax.x = std::max(vmax.x, v.position.x);
		//		vmax.y = std::max(vmax.y, v.position.y);
		//		vmax.z = std::max(vmax.z, v.position.z);
		//	}
		//}
		bool isMetaDirty = false; // 메타데이터 저장 필요 여부

		for (UInt32 i = 0; i < modelData->meshes.size(); i++)
		{
			auto& meshData = modelData->meshes[i];
			String meshName = meshData.name;
			if (meshName.empty())
			{
				meshName = "Unnamed_" + std::to_string(i);
			}


			AssetHandle meshHandle;
			//mesh이름과 관련된 데이터가 있으면 메쉬핸들만 가져온다.
			if (metaNode["SubAssets"][meshName])
			{
				String handleStr = metaNode["SubAssets"][meshName]["Handle"].as<String>();
				meshHandle = AssetHandle(handleStr); // 기존 핸들 사용
			}
			else //없으면 
			{
				String pathMapKey = pathString + '#' + meshName; // 파일명이 중복될 수 있으므로

				//Create Subasset Mesh Metadata
				meshHandle = AssetHandle::Generate(); // 랜덤 UUID 생성

				//새로운 메쉬정보를 등록
				AssetMetadata metadata = AssetMetadata();
				metadata.handle = meshHandle;
				metadata.filePath = pathMapKey;
				metadata.type = AssetType::Mesh;
				AssetManager::Register(metadata);

				// 새로운 노드 객체 생성
				YAML::Node newEntry;
				newEntry["Handle"] = meshHandle.ToString();
				newEntry["Path"] = pathMapKey;
				newEntry["Type"] = "Mesh";
				newEntry["Name"] = meshName;

				metaNode["SubAssets"][meshName] = newEntry;
				isMetaDirty = true;
			}

			Shared<Mesh> mesh = Mesh::Create(meshData);
			mesh->SetAssetHandle(meshHandle);
			mesh->SetAssetName(meshName);
			AssetManager::AddLoadedAsset(meshHandle, mesh);
			newModel->AddMesh(meshHandle);
		}

		if (isMetaDirty)
		{
			std::ofstream fout(metaFilePathString);
			fout << metaNode;
			fout.close();
		}

		Path materialDir = modelPath.parent_path() / "Materials";
		if (!FileSystem::exists(materialDir))
			FileSystem::create_directory(materialDir);

		for (UInt32 i = 0; i < modelData->materials.size(); i++)
		{
			auto& materialData = modelData->materials[i];

			String materialName = materialData.name;
			if (materialName.empty())
			{
				materialName = "Unnamed_" + std::to_string(i);
			}

			Path materialPath = materialDir / (materialName + ".ddmat");
			String materialPathString = materialPath.generic_string();

			AssetHandle materialHandle;
			if (FileSystem::exists(materialPath))
			{
				Shared<Material> existingMaterial = AssetManager::GetAssetByPath<Material>(materialPathString);
				if (existingMaterial)
				{
					materialHandle = existingMaterial->GetAssetHandle();
					existingMaterial->SetAssetName(materialName);
				}
				else
				{
					//TODO:error msg
				}
			}
			else
			{
				Shared<Material> newMaterial = Material::Create(ResourceManager::GetResource<PipelineState>("GBufferPSO"));
				AssetHandle albedo = AssetManager::GetAssetHandleByPath(modelData->materials[i].albedoMapPath);
				AssetHandle normal = AssetManager::GetAssetHandleByPath(modelData->materials[i].normalMapPath);
				AssetHandle roughness = AssetManager::GetAssetHandleByPath(modelData->materials[i].roughnessMapPath);
				AssetHandle metallic = AssetManager::GetAssetHandleByPath(modelData->materials[i].metallicMapPath);
				AssetHandle ao = AssetManager::GetAssetHandleByPath(modelData->materials[i].AOMapPath);

				if (!albedo.IsValid())
				{
					albedo = AssetDefaults::DefaultAlbedoHandle;
				}

				if (!normal.IsValid())
				{
					normal = AssetDefaults::DefaultNormalHandle;
				}

				if (!roughness.IsValid())
				{
					roughness = AssetDefaults::DefaultRoughnessHandle;
				}

				if (!metallic.IsValid())
				{
					metallic = AssetDefaults::DefaultMetallicHandle;
				}
				if (!ao.IsValid())
				{
					ao = AssetDefaults::DefaultAOHandle;
				}

				newMaterial->SetTextureBinding("mat_AlbedoMap", albedo);
				newMaterial->SetTextureBinding("mat_NormalMap", normal);
				newMaterial->SetTextureBinding("mat_RoughnessMap", roughness);
				newMaterial->SetTextureBinding("mat_MetallicMap", metallic);
				newMaterial->SetTextureBinding("mat_AOMap", ao);

				materialHandle = AssetHandle::Generate();
				newMaterial->SetAssetHandle(materialHandle);
				newMaterial->SetAssetName(materialName);

				AssetMetadata matMetadata;
				matMetadata.handle = materialHandle;
				matMetadata.filePath = materialPathString;
				matMetadata.type = AssetType::Material;
				matMetadata.name = materialName;
				AssetManager::CreateMetaDataFile(matMetadata);
				AssetManager::Register(matMetadata); // 여기서 .ddmat.ddmeta 파일이 생성
				AssetManager::AddLoadedAsset(materialHandle, newMaterial);

				YAML::Emitter out;
				out << YAML::BeginMap;
				out << YAML::Key << "Material";
				out << YAML::BeginMap;
				out << YAML::Key << "PSO" << YAML::Value << "GBufferPSO";

				//// 파라미터 저장
				//out << YAML::Key << "Parameters";
				//out << YAML::BeginMap;
				//out << YAML::Key << "Roughness" << material->GetRoughness();
				//out << YAML::Key << "AlbedoColor" << material->GetAlbedoColor(); // Vector는 오버로딩 필요
				//out << YAML::EndMap;

				// 텍스처 저장
				out << YAML::Key << "Textures";
				out << YAML::BeginMap;
				for (const auto& [name, textureBinding] : newMaterial->GetTextureBindings())
				{
					out << YAML::Key << name << YAML::Value << textureBinding.cache->GetAssetHandle().ToString();
				}
				out << YAML::EndMap;
				out << YAML::EndMap;
				out << YAML::EndMap;

				std::ofstream fout(materialPath);
				fout << out.c_str();
				fout.close();
			}
			newModel->AddMaterial(materialHandle);
			//Shared<Material> newMat = Material::Create(ResourceManager::GetResource<PipelineState>("ForwardPSO"));
			//materials.push_back(newMat);
		}

		return newModel;
	}
	Shared<Shader> AssetImporter::LoadShader(const AssetMetadata& _metaData)
	{
		// 지원하는 확장자인지 확인
		Path entryPath = _metaData.filePath;
		String pathString = entryPath.string();
		String shaderName = entryPath.stem().string();
		String extension = entryPath.extension().string();
		ShaderType shaderType;
		if (pathString.find("VS.") != std::string::npos)
		{
			shaderType = ShaderType::Vertex;
		}
		if (pathString.find("PS.") != std::string::npos)
		{
			shaderType = ShaderType::Pixel;
		}

		if (pathString.find("HS.") != std::string::npos)
		{
			shaderType = ShaderType::Hull;
		}

		if (pathString.find("DS.") != std::string::npos)
		{
			shaderType = ShaderType::Domain;
		}
		if (pathString.find("GS.") != std::string::npos)
		{
			shaderType = ShaderType::Geometry;
		}
		Shared<Shader> newShader = Shader::Create(pathString, shaderType, ShaderLoadMode::File);

		return newShader;
	}

	Shared<Material> AssetImporter::LoadMaterial(const AssetMetadata& _metaData)
	{
		Path materialPath = _metaData.filePath;

		String pathString = materialPath.generic_string();
		String extension = materialPath.extension().string();
		String metaFilePathString = pathString + ".ddmeta";

		YAML::Node metaNode = YAML::LoadFile(pathString);
		if (!metaNode["Material"])
		{
			//Error is not material
			return nullptr;
		}
		YAML::Node matNode = metaNode["Material"];
		String PSO = matNode["PSO"].as<String>();
		Shared<Material> newMaterial = Material::Create(ResourceManager::GetResource<PipelineState>(PSO));
		YAML::Node textureNode = matNode["Textures"];
		if (textureNode)
		{
			// YAML::const_iterator를 사용하여 Key(슬롯이름)와 Value(UUID)를 쌍으로 가져오기
			for (auto it = textureNode.begin(); it != textureNode.end(); ++it)
			{
				String slotName = it->first.as<String>();      // 예) "AlbedoTexture"
				String handleStr = it->second.as<String>();    // 예) "de27a743-..."

				// 문자열 UUID를 AssetHandle로 변환
				AssetHandle handle(handleStr);

				if (handle.IsValid())
				{
					newMaterial->SetTextureBinding(slotName, handle);
				}
			}
		}

		return newMaterial;

	}
}

