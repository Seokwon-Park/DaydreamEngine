#pragma once

#include "vulkan/vulkan.h"
#include "Steins/Render/GraphicsDevice.h"

namespace Steins
{
	class VulkanGraphicsDevice :public GraphicsDevice
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;
		virtual void SwapBuffers() override;

		virtual void SetPrimitiveTopology(PrimitiveTopology _primitiveTopology) override;

	};
}

