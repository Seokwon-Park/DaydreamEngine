#include "DaydreamPCH.h"
#include "ShaderGroupManager.h"

namespace Daydream
{
	ShaderGroupManager::ShaderGroupManager()
	{
	}
	void ShaderGroupManager::CreateEssentialShaderGroups()
	{
		registry["Model"] = ShaderGroup::Create("ModelVS", "ModelPS");
		registry["Cubemap"] = ShaderGroup::Create("CubemapVS", "CubemapPS");
		registry["Equirectangular"] = ShaderGroup::Create("EquirectangularVS", "EquirectangularPS");
		registry["Sprite"] = ShaderGroup::Create("SpriteVS", "SpritePS");
	}
}