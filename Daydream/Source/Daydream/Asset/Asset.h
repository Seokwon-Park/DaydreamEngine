#pragma once

#include "Daydream/Core/UUID.h"

namespace Daydream
{
	enum class AssetType
	{
		None,
		Texture2D,
		TextureCube,
		Shader,
		Scene,
		Model,
		Mesh,
		Material,
		Script
	};

	class Asset
	{
	public:
		Asset();
		~Asset();

		inline void SetAssetHandle(AssetHandle _id) { id = _id; }
		inline AssetHandle GetAssetHandle() { return id; }

		inline void SetAssetName(String _name) { name = _name; }
		inline const String& GetAssetName() const 
		{ 
			if (name.empty())
			{
				DAYDREAM_CORE_INFO("No Name");
			}
			return name; 
		}

		inline AssetType GetAssetType() const { return type; }
	protected:

	private:
		AssetType type = AssetType::None;
		AssetHandle id = AssetHandle();
		String name = "";
	};
}
