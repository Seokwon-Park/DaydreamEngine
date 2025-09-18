#pragma once

#include "Daydream/Graphics/Resources/Sampler.h"
#include "Daydream/Core/RegistryBase.h"

namespace Daydream
{
	class SamplerManager : public RegistryBase<Sampler>
	{
	public:
		SamplerManager();

		void CreateEssentialSamplers();
	};
}