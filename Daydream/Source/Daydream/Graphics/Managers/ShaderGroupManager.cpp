#include "DaydreamPCH.h"
#include "ShaderGroupManager.h"

namespace Daydream
{
	ShaderGroupManager::ShaderGroupManager()
	{
	}
	void ShaderGroupManager::CreateEssentialShaderGroups()
	{
		resourceCache["Model"] = ShaderGroup::Create("ModelVS", "ModelPS");
		resourceCache["Cubemap"] = ShaderGroup::Create("CubemapVS", "CubemapPS");
		resourceCache["Sprite"] = ShaderGroup::Create("SpriteVS", "SpritePS");
	}
}