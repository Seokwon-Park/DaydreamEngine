#pragma once

#include "ResourceRegistry.h"

namespace Daydream
{
	class ShaderGroup;

	class ShaderGroupRegistry : public ResourceRegistry<ShaderGroup>
	{
	public:
        ShaderGroupRegistry();

		virtual void CreateBuiltinResources() override;

	private:

	};
}

