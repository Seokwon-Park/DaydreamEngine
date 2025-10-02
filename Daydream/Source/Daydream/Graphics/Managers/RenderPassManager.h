#pragma once

#include "Daydream/Core/RegistryBase.h"
#include "Daydream/Graphics/Core/RenderPass.h"


namespace Daydream
{
	class RenderPassManager : public RegistryBase<String, Shared<RenderPass>>
	{
	public:
		RenderPassManager();

		void CreateEssentialRenderPasses();
	private:
	};
}