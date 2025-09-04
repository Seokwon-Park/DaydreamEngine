#pragma once

#include "Daydream/Graphics/Core/RenderContext.h"
#include "VulkanRenderDevice.h"

namespace Daydream
{
	class VulkanGraphicsContext : public RenderContext
	{
	public:
		VulkanGraphicsContext(VulkanRenderDevice* _device);

		virtual void Begin() override;
		virtual void SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height) override {};
		virtual void SetClearColor(const Color& _color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;
	private:
		VulkanRenderDevice* device;
	};
}