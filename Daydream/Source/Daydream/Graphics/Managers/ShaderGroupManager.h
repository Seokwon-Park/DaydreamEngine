#pragma once

#include "Daydream/Core/RegistryBase.h"
#include "Daydream/Graphics/Resources/ShaderGroup.h"

namespace Daydream
{
	class ShaderGroupManager : public RegistryBase<String, Shared<ShaderGroup>>
	{
	public:
		ShaderGroupManager();

		void CreateEssentialShaderGroups();
	};
}

