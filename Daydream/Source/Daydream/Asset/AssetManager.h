#pragma once

#include "Asset.h"

namespace Daydream
{
	enum class LoadPhase {
		Early,		// 0: 텍스처, 쉐이더 등 (가장 먼저)
		Normal,     // 1: 메테리얼, 스크립트
		Late,		// 2: 레벨, 캐릭터 (마지막)
		MAX
	};

	struct AssetMetadata
	{
		AssetHandle handle; // 이 에셋의 고유 ID
		Path filePath; // 디스크 상의 실제 경로 (예: "Assets/Textures/Player.png")
		AssetType type = AssetType::None; // (enum: Texture, Mesh, Scene, Material...)
		String name;

		HashMap<String, AssetMetadata> subAssets;

		bool IsValid()
		{
			return type != AssetType::None && handle.IsValid();
		}
	};

	class AssetManager
	{
	public:
		static AssetManager& GetInstance()
		{
			return *instance;
		}

		static void Init();
		static void Shutdown();

		static void LoadAssetMetadataFromDirectory(const Path& _directoryPath, bool _isRecursive = true);
		static void LoadAssets(LoadPhase _phase);
		static void CreateBuiltinAssets();

		static const AssetMetadata& GetAssetMetadata(AssetHandle _handle);
		static AssetHandle GetAssetHandleByPath(const Path& _path);

		static AssetType GetAssetTypeFromExtension(String _ext);

		inline static AssetType GetAssetTypeFromPath(const Path& _path)
		{
			String ext = _path.extension().string();
			return GetAssetTypeFromExtension(ext);
		}

		template<typename AssetType>
		static Shared<AssetType> GetAsset(AssetHandle _uuid)
		{
			auto itr = instance->loadedAssetCache.find(_uuid);
			if (itr != instance->loadedAssetCache.end())
			{
				return SharedCast<AssetType>(itr->second);
			}
			
			Shared<Asset> newAsset = instance->LoadAssetCache(_uuid);

			if (newAsset == nullptr)
			{
				return nullptr;
			}

			newAsset->SetAssetHandle(_uuid);
			instance->loadedAssetCache[_uuid] = newAsset;

			return std::static_pointer_cast<AssetType>(newAsset);
		}

		template<typename AssetType>
		static Shared<AssetType> GetAssetByPath(const Path& _path)
		{
			if (_path.empty()) return nullptr;
			String path = _path.generic_string();
			auto itr = instance->assetPathMap.find(path);
			if (itr == instance->assetPathMap.end())
			{
				DAYDREAM_CORE_WARN("Wrong Path");
				return nullptr;
			}
			AssetHandle uuid = itr->second;
			return GetAsset<AssetType>(uuid);
		}

		inline static void Register(const AssetMetadata& _assetMetadata)
		{
			instance->RegisterAsset(_assetMetadata);
		}

		inline static void AddLoadedAsset(AssetHandle _handle, Shared<Asset> _asset)
		{
			instance->loadedAssetCache[_handle] = _asset;
		}

		inline static void CreateMetaDataFile(const AssetMetadata& _metadata)
		{
			instance->CreateMetaDataFileInternal(_metadata);
		}
		static String AssetTypeToString(AssetType _type);

		static Array<AssetMetadata> GetAssetsByType(AssetType type);
	protected:
	private:
		AssetManager();

		void CreateBuiltinTexture2D();

		void ProcessDirectory(const Path& _directoryPath, bool _isRecursive = true);
		void ProcessFile(const Path& _filePath, AssetType _assetType);
		void RegisterAsset(const AssetMetadata& _metadata);
		AssetMetadata LoadMetadata(const Path& _path);
		Shared<Asset> LoadAssetCache(AssetHandle _uuid);
		void CreateMetaDataFileInternal(const AssetMetadata& _metadata);

		inline AssetType StringToAssetType(String _typeString)
		{
			return assetTypeMap.at(_typeString);
		}

		HashMap<AssetHandle, AssetMetadata> assetRegistry;
		HashMap<String, AssetHandle> assetPathMap;
		HashMap<AssetHandle, Shared<Asset>> loadedAssetCache;

		inline static AssetManager* instance = nullptr;

		static const SortedMap<String, AssetType> assetExtensionMap;
		static const SortedMap<String, AssetType> assetTypeMap;
	};

}
