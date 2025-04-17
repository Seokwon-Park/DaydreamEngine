#pragma once

#include "Steins/Render/SwapChain.h"
#include "Steins/Render/GraphicsDevice.h"

namespace Steins
{
	class VulkanSwapChain : public SwapChain
	{
	public:
		VulkanSwapChain(GraphicsDevice* _device, SwapChainDesc* _desc, SteinsWindow* _window);
	private:
		

	};
}