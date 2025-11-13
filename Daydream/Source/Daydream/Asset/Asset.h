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
		Script
	};

	class Asset
	{
	public:
		Asset();
		~Asset();

		inline void SetUUID(AssetHandle _id) { id = _id; }
		inline AssetHandle GetAssetHandle() { return id; }
	protected:

	private:
		AssetType type;
		AssetHandle id;
	};
}
