#include "DaydreamPCH.h"
#include "AssetManager.h"

#include "AssetImporter.h"

#include "Daydream/Core/UUID.h"
#include "yaml-cpp/yaml.h"

namespace Daydream
{
	const SortedMap<String, AssetType> AssetManager::assetExtensionMap = {
		// 텍스처
		{".png", AssetType::Texture2D},
		{".jpg", AssetType::Texture2D},
		{".jpeg", AssetType::Texture2D},
		{".tga", AssetType::Texture2D},
		{".hdr", AssetType::Texture2D},

		// 씬
		{".ddscene", AssetType::Scene}, // 예시

		{".hlsl", AssetType::Shader}, 

		// 메쉬
		{".fbx", AssetType::Model},
		{".gltf", AssetType::Model},
		{".obj", AssetType::Model},

		// 스크립트
	};

	namespace 
	{
		const SortedMap<String, AssetType> assetTypeMap =
		{
			{"Texture2D", AssetType::Texture2D},
			{"Model", AssetType::Model},
			{"Shader", AssetType::Shader},
		};


		String AssetTypeToString(AssetType _type)
		{
			switch (_type)
			{
			case AssetType::None:
				break;
			case AssetType::Texture2D:
				return "Texture2D";
				break;
			case AssetType::Scene:
				break;
			case AssetType::Shader:
				return "Shader";
				break;
			case AssetType::Model:
				return "Model";
				break;
			case AssetType::Script:
				break;
			default:
				break;
			}
			return "";
		}

		AssetType StringToAssetType(String _typeString)
		{
			return assetTypeMap.at(_typeString);
		}
	}
	AssetManager::AssetManager()
	{

	}

	void AssetManager::Init()
	{
		instance = new AssetManager();
	}

	void AssetManager::Shutdown()
	{
		delete instance;
	}

	void AssetManager::LoadAssetDataFromDirectory(const Path& _directoryPath, bool _isRecursive)
	{
		instance->ProcessDirectory(_directoryPath, _isRecursive);
	}

	void AssetManager::RegisterAsset(const Path& _path)
	{
		Path metaFilePath = _path;
		metaFilePath += ".ddmeta";

		AssetMetadata metadata = LoadMetadata(metaFilePath);
		if (!metadata.IsValid())
		{
			// 메타파일이 손상되었거나 유효하지 않음
			return;
		}

		// 파일 경로가 메타데이터와 다를 수 있으므로(파일 이동 감지),
		// 실제 파일 경로로 업데이트
		metadata.filePath = _path; // TODO: 상대 경로로 변환해야 함

		// 레지스트리에 등록
		if (assetRegistry.find(metadata.handle) == assetRegistry.end())
		{
			assetRegistry[metadata.handle] = metadata;
			assetPathMap[metadata.filePath.generic_string()] = metadata.handle;
		}
	}

	AssetMetadata AssetManager::LoadMetadata(const Path& _metaFilePath)
	{
		YAML::Node data = YAML::LoadFile(_metaFilePath.string());
		if (!data["Handle"])
		{
			return AssetMetadata(); // 유효하지 않음
		}

		//String tmp = data["Type"].as<String>();
		//if (tmp == "Texture")
		//{
		//	data["Type"] = "Texture2D";
		//	std::ofstream fout(_metaFilePath);
		//	fout << data;
		//	fout.close();
		//}

		AssetMetadata metadata;
		metadata.handle = AssetHandle(data["Handle"].as<String>());
		metadata.filePath = data["Path"].as<String>();
		metadata.type = StringToAssetType(data["Type"].as<String>());

		return metadata;
	}

	Shared<Asset> AssetManager::LoadAssetCache(AssetHandle _uuid)
	{
		// 1. 메타데이터 레지스트리에서 애셋 정보 찾기
		auto itr = assetRegistry.find(_uuid);
		if (itr == assetRegistry.end())
		{
			DAYDREAM_CORE_WARN("Cannot find AssetMetaData!");
			return nullptr;
		}

		const AssetMetadata& metadata = itr->second;

		// 2. 메타데이터의 'Type'에 따라 적절한 임포터 호출
		Shared<Asset> loadedAsset = nullptr;
		switch (metadata.type)
		{
		case AssetType::Texture2D:
			loadedAsset = AssetImporter::LoadTexture2D(metadata.filePath);
			break;
		case AssetType::TextureCube:
			//loadedAsset = AssetImporter::LoadTextureCube(metadata.FilePath);
			break;
		case AssetType::Model:
			loadedAsset = AssetImporter::LoadModel(metadata.filePath);
			break;
			// ... 기타 애셋 타입
		case AssetType::Shader:
			loadedAsset = AssetImporter::LoadShader(metadata.filePath);
			break;
		default:
			// 로드할 수 없는 타입
			return nullptr;
		}

		if (loadedAsset == nullptr)
		{
			// 파일이 없거나, 임포트 중 손상됨
			return nullptr;
		}

		// 3. 로드 성공! 캐시에 저장
		loadedAssetCache[_uuid] = loadedAsset;

		// 4. 로드된 애셋(Shared<Asset>) 반환
		return loadedAsset;
	}

	AssetType AssetManager::GetAssetTypeFromPath(const Path& _path)
	{
		String ext = _path.extension().string();
		auto itr = assetExtensionMap.find(ext);
		if (itr == assetExtensionMap.end())
		{
			return AssetType::None;
		}
		return itr->second;
	}

	void AssetManager::ProcessDirectory(const Path& _directoryPath, bool _isRecursive)
	{
		auto dirItr = FileSystem::directory_iterator(_directoryPath);
		for (auto dirEntry : dirItr)
		{
			Path path = dirEntry.path();
			String ext = path.extension().string();
			if (ext == ".ddmeta") continue;
			if (dirEntry.is_directory())
			{
				if (_isRecursive)
				{
					ProcessDirectory(path, _isRecursive);
				}
				else
				{
					continue;
				}
			}
			else
			{
				AssetType type = GetAssetTypeFromPath(path);
				if (type == AssetType::None) continue;
				ProcessFile(path, type);
			}
		}
	}

	void AssetManager::ProcessFile(const Path& _filePath, AssetType _assetType)
	{
		Path metaFilePath = _filePath;
		metaFilePath += ".ddmeta";
		if (FileSystem::exists(metaFilePath))
		{
			RegisterAsset(_filePath);
		}
		else
		{
			AssetMetadata metadata;
			metadata.handle = AssetHandle();
			metadata.filePath = _filePath.string(); // TODO: 상대 경로로 변환해야 함
			metadata.type = _assetType;
			//Create metafile
			CreateMetaDataFile(metadata);
		}
	}

	void AssetManager::CreateMetaDataFile(const AssetMetadata& _metadata)
	{
		Path metaFilePath = _metadata.filePath;
		metaFilePath += ".ddmeta";

		YAML::Emitter out;
		out << YAML::BeginMap;
		// out << YAML::Key << "Handle" << YAML::Value << _metadata.Handle; (UUID를 문자열로)
		out << YAML::Key << "Handle" << YAML::Value << _metadata.handle.ToString();
		out << YAML::Key << "Path" << YAML::Value << _metadata.filePath.string();
		// AssetType을 문자열로 변환하는 함수가 필요함
		out << YAML::Key << "Type" << YAML::Value << AssetTypeToString(_metadata.type);
		out << YAML::EndMap;

		std::ofstream fout(metaFilePath);
		fout << out.c_str();
		fout.close();
	}
}

