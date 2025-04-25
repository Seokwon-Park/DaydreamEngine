#pragma once

#include "Steins/Enum/RendererEnums.h"
#include "vulkan/vulkan.h"

namespace Steins
{
	class GraphicsUtil
	{
	public:
		static DXGI_FORMAT RenderFormatToDXGIFormat(RenderFormat _format);
		static VkFormat RenderFormatToVkFormat(RenderFormat _format);

		static std::string GetVendor(int _vendorCode);
		
	};
	

}