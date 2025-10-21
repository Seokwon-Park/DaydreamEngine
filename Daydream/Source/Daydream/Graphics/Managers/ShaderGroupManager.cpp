#include "DaydreamPCH.h"
#include "ShaderGroupManager.h"

namespace Daydream
{
	ShaderGroupManager::ShaderGroupManager()
	{
	}
	void ShaderGroupManager::CreateEssentialShaderGroups()
	{
		//registry["Model"] = ShaderGroup::Create("ModelVS", "ModelPS");
		registry["Model"] = ShaderGroup::Create("ModelVS", "PBRModelPS");
		registry["Cubemap"] = ShaderGroup::Create("CubemapVS", "CubemapPS");
		registry["Equirectangular"] = ShaderGroup::Create("CubemapVS", "EquirectangularPS");
		registry["Irradiance"] = ShaderGroup::Create("CubemapVS", "IrradiancePS");
		registry["Prefilter"] = ShaderGroup::Create("CubemapVS", "PrefilterPS");
		registry["Mip"] = ShaderGroup::Create("ResizeVS", "ResizePS");
		registry["Sprite"] = ShaderGroup::Create("SpriteVS", "SpritePS");
		registry["Resize"] = ShaderGroup::Create("ResizeVS", "ResizePS");
		registry["BRDF"] = ShaderGroup::Create("BRDFVS", "BRDFPS");
	}
}