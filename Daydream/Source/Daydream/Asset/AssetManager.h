#pragma once

#include "Asset.h"

namespace Daydream
{
	struct AssetMetadata
	{
		Daydream::AssetHandle handle; // 이 에셋의 고유 ID
		Path filePath; // 디스크 상의 실제 경로 (예: "Assets/Textures/Player.png")
		AssetType type = AssetType::None; // (enum: Texture, Mesh, Scene, Material...)

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

		static void LoadAssetDataFromDirectory(const Path& _directoryPath, bool _isRecursive = true);

		template<typename AssetType>
		static Shared<AssetType> GetAsset(AssetHandle _uuid)
		{
			auto itr = instance->loadedAssetCache.find(_uuid);
			if (itr != instance->loadedAssetCache.end())
			{
				return std::static_pointer_cast<AssetType>(itr->second);
			}
			Shared<Asset> newAsset = instance->LoadAssetCache(_uuid);
			newAsset->SetUUID(_uuid);

			if (newAsset == nullptr)
			{
				// 로딩 실패 (유효하지 않은 UUID, 파일 없음 등)
				return nullptr;
			}

			// (newAsset은 이제 loadedAssetCache에도 저장되어 있음)
			return std::static_pointer_cast<AssetType>(newAsset);
		}

		template<typename AssetType>
		static Shared<AssetType> GetAssetByPath(const Path& _path)
		{
			String path = _path.generic_string();
			auto itr = instance->assetPathMap.find(path);
			if (itr == instance->assetPathMap.end())
			{
				return nullptr;
			}
			AssetHandle uuid = itr->second;
			return GetAsset<AssetType>(uuid);
		}
	protected:
	private:
		AssetManager();

		void ProcessDirectory(const Path& _directoryPath, bool _isRecursive = true);
		void ProcessFile(const Path& _filePath, AssetType _assetType);
		void RegisterAsset(const Path& _path);
		AssetMetadata LoadMetadata(const Path& _path);
		Shared<Asset> LoadAssetCache(AssetHandle _uuid);
		AssetType GetAssetTypeFromPath(const Path& _path);

		void CreateMetaDataFile(const AssetMetadata& _metadata);

		HashMap<AssetHandle, AssetMetadata> assetRegistry;
		HashMap<String, AssetHandle> assetPathMap;
		HashMap<AssetHandle, Shared<Asset>> loadedAssetCache;

		inline static AssetManager* instance = nullptr;

		static const SortedMap<String, AssetType> assetExtensionMap;
	};
}
