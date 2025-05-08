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

		//RenderFormat
		static DXGI_FORMAT RenderFormatToDXGIFormat(RenderFormat _format);
		static VkFormat RenderFormatToVkFormat(RenderFormat _format);

		//ShaderCompile
		static DXShaderCompileParam GetDXShaderCompileParam(ShaderType _type);

		//ShaderType, ShaderStage
		static GLenum GetGLShaderStage(ShaderType _type);
		static GLenum GetGLShaderType(ShaderType _type);
		static VkShaderStageFlagBits GetVKShaderStage(ShaderType _type);

		static std::string GetVendor(int _vendorCode);
	};
}