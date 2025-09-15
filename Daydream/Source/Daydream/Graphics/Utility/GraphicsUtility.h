#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "spirv_cross/spirv_cross.hpp"

#define VK_SUCCEEDED(x) x == VK_SUCCESS

namespace Daydream::GraphicsUtility
{
	bool HasFlag(RenderBindFlags _target, RenderBindFlags _flags);

	UInt32 GetRenderFormatSize(RenderFormat _format);
	

	//VkFormat ConvertRenderFormatToVkFormat(RenderFormat _format);

	RenderFormat ConvertSPIRVTypeToRenderFormat(spirv_cross::SPIRType::BaseType _baseType, UInt32 _componentCount);


	//ShaderCompile
	String GetShaderEntryPointName(ShaderType _type);
	WideString GetShaderEntryPointNameW(ShaderType _type);
	String GetShaderTargetName(ShaderType _type, String _version);
	WideString GetShaderTargetNameW(ShaderType _type, WideString _version);

	//ShaderType, ShaderStage


	

	String GetVendor(UInt32 _vendorCode);
}