#pragma once

#include "Steins/Enum/RendererEnums.h"
#include "vulkan/vulkan.h"
#include "glad/glad.h"
#include "spirv_cross/spirv_cross.hpp"

#define VK_SUCCEEDED(x) x == VK_SUCCESS

namespace Steins
{
	struct DXShaderCompileParam
	{
		std::string entryPoint;
		std::string target;
	};

	struct DXShaderCompileParamW
	{
		std::wstring entryPoint;
		std::wstring target;
	};

	class GraphicsUtil
	{
	public:
		static UInt32 GetRenderFormatSize(RenderFormat _format);

		static DXGI_FORMAT ShaderDataTypeToDXGIFormat(ShaderDataType type);

		//RenderFormat
		static DXGI_FORMAT RenderFormatToDXGIFormat(RenderFormat _format);
		static VkFormat RenderFormatToVkFormat(RenderFormat _format);
		static RenderFormat ConvertSPIRVTypeToRenderFormat(spirv_cross::SPIRType::BaseType _baseType, UInt32 _componentCount);


		//ShaderCompile
		static String GetShaderEntryPointName(ShaderType _type);
		static WideString GetShaderEntryPointNameW(ShaderType _type);
		static String GetShaderTargetName(ShaderType _type);
		static WideString GetShaderTargetNameW(ShaderType _type, WideString _version);

		//ShaderType, ShaderStage
		static GLenum GetGLShaderStage(ShaderType _type);
		static GLenum GetGLShaderType(ShaderType _type);
		static D3D12_SHADER_VISIBILITY GetDX12ShaderVisibility(ShaderType _type);
		static VkShaderStageFlagBits GetVKShaderStage(ShaderType _type);

		static std::string GetVendor(int _vendorCode);
	};
}