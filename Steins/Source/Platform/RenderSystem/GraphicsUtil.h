#pragma once

#include "Steins/Enum/RendererEnums.h"
#include "vulkan/vulkan.h"
#include "glad/glad.h"

#define VK_SUCCEEDED(x) x == VK_SUCCESS

namespace Steins
{
	struct DXShaderCompileParam
	{
		std::string entryPoint;
		std::string target;
	};

	class GraphicsUtil
	{
	public:
		static DXGI_FORMAT ShaderDataTypeToDXGIFormat(ShaderDataType type);


		static DXGI_FORMAT RenderFormatToDXGIFormat(RenderFormat _format);
		static VkFormat RenderFormatToVkFormat(RenderFormat _format);

		static DXShaderCompileParam GetDXShaderCompileParam(ShaderType _type);

		static std::map<ShaderType, GLenum> openGLShaderStageMap;
		static std::map<ShaderType, GLenum> openGLShaderTypeMap;
		static std::map<ShaderType, VkShaderStageFlagBits> vulkanShaderStageMap;

		static std::string GetVendor(int _vendorCode);
	};
}