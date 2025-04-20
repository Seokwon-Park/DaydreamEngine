#pragma once

#include "Steins/Render/RendererAPI.h"
#include "VulkanGraphicsDevice.h"

namespace Steins
{
	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI(GraphicsDevice* _device);

		// Inherited via RendererAPI
		void SetClearColor(const Color& _color) override;
		void Clear() override;
		void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;
	};
}