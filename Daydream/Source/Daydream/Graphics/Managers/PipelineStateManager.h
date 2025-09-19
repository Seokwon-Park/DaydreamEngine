#pragma once

#include "Daydream/Core/RegistryBase.h"


namespace Daydream
{
	class PipelineState;

	class PipelineStateManager : public RegistryBase<String, Shared<PipelineState>>
	{
	public:
		PipelineStateManager();

		void CreateEssentialPipelineStates();
	private:
	};
}