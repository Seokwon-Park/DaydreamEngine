#pragma once

#include "Steins/Enum/RendererEnums.h"
#include "vulkan/vulkan.h"
#include "glad/glad.h"

#define VK_SUCCEEDED(x) x == VK_SUCCESS

namespace Steins
{
	class GraphicsUtil
	{
	public:
		static DXGI_FORMAT RenderFormatToDXGIFormat(RenderFormat _format);
		static VkFormat RenderFormatToVkFormat(RenderFormat _format);

		static std::map<ShaderType, GLenum> openGLShaderStageMap;
		static std::map<ShaderType, GLenum> openGLShaderTypeMap;
		static std::map<ShaderType, VkShaderStageFlagBits> vulkanShaderStageMap;

		static std::string GetVendor(int _vendorCode);
		
	};
	

}