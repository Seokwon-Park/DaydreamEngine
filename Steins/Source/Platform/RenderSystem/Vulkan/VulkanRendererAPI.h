#pragma once

#include "Steins/Render/RendererAPI.h"
#include "VulkanGraphicsDevice.h"

namespace Steins
{
	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI(GraphicsDevice* _device);

		virtual void Init() override;
		virtual void SetClearColor(const Color& _color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;
	private:
		VulkanGraphicsDevice* device;
	};
}