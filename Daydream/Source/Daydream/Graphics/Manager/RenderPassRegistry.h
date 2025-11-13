#pragma once

#include "ResourceRegistry.h"



namespace Daydream
{
	class RenderPass;

	class RenderPassRegistry : public ResourceRegistry<RenderPass>
	{
	public:
		RenderPassRegistry() {};

		virtual void CreateBuiltinResources() override;
	private:
	};
}