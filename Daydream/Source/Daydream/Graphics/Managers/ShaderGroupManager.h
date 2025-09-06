#pragma once

#include "Daydream/Core/ResourceManagerBase.h"
#include "Daydream/Graphics/Resources/ShaderGroup.h"

namespace Daydream
{
	class ShaderGroupManager : public ResourceManagerBase<ShaderGroup>
	{
	public:
		ShaderGroupManager();

		void CreateEssentialShaderGroups();
	};
}

