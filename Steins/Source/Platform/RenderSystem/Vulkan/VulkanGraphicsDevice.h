#pragma once

#include "vulkan/vulkan.h"
#include "Steins/Render/GraphicsDevice.h"

namespace Steins
{
	class VulkanGraphicsDevice :public GraphicsDevice
	{
	public:
		void Init() override;
		void Shutdown() override;
		void Render() override;
		void SwapBuffers() override;
	};
}

