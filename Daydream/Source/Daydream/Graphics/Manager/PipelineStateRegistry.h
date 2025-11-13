#pragma once

#include "ResourceRegistry.h"


namespace Daydream
{
	class PipelineState;

	class PipelineStateRegistry : public ResourceRegistry<PipelineState>
	{
	public:
		PipelineStateRegistry();

		virtual void CreateBuiltinResources() override;
	private:
	};
}