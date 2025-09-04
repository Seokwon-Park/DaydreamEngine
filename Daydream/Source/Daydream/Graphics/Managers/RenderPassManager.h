#pragma once

#include "Daydream/Graphics/Core/RenderPass.h"


namespace Daydream
{
	class RenderPassManager
	{
	public:
		RenderPassManager();

		Shared<RenderPass> Get(const String& _name)
		{
			return renderPassCache[_name];
		}

		void CreateEssentialRenderPasses();
	private:
		HashMap<String, Shared<RenderPass>> renderPassCache;
	};
}