#pragma once

#include "Daydream/Graphics/Resources/Sampler.h"
#include "Daydream/Core/ResourceManagerBase.h"

namespace Daydream
{
	class SamplerManager : public ResourceManagerBase<Sampler>
	{
	public:
		SamplerManager();

		void CreateEssentialSamplers();
	};
}