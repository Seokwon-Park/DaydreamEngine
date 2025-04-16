#pragma once

#include "Steins/Render/SwapChain.h"

namespace Steins
{
	class VulkanSwapChain : public SwapChain
	{
		VulkanSwapChain(SwapChainDesc* _desc, SteinsWindow* _window);
	};
}