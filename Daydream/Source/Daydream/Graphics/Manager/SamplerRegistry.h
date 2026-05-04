#pragma once

#include "Daydream/Graphics/Resources/Sampler.h"
#include "ResourceRegistry.h"

namespace Daydream
{
	class SamplerRegistry : public ResourceRegistry<Sampler>
	{
	public:
		SamplerRegistry();

		virtual void CreateBuiltinResources() override;

		inline static Shared<Sampler> LinearRepeat;
		inline static Shared<Sampler> LinearClampToEdge;
		inline static Shared<Sampler> NearestRepeat;
		inline static Shared<Sampler> NearestClampToEdge;
	};
}