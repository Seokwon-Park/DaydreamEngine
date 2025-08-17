#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "vulkan/vulkan.hpp"
#include "glad/glad.h"
#include "spirv_cross/spirv_cross.hpp"

#define VK_SUCCEEDED(x) x == VK_SUCCESS

namespace Daydream
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

	namespace GraphicsUtil
	{
		bool HasFlag(RenderBindFlags _target, RenderBindFlags _flags);

		UInt32 GetRenderFormatSize(RenderFormat _format);

		DXGI_FORMAT ShaderDataTypeToDXGIFormat(ShaderDataType type);

		//RenderFormat
		DXGI_FORMAT ConvertRenderFormatToDXGIFormat(RenderFormat _format);
		//VkFormat ConvertRenderFormatToVkFormat(RenderFormat _format);
		vk::Format ConvertRenderFormatToVkFormat(RenderFormat _format);
		UInt32 ConvertRenderFormatToGLFormat(RenderFormat _format);
		UInt32 ConvertRenderFormatToGLDataFormat(RenderFormat _format);
		RenderFormat ConvertSPIRVTypeToRenderFormat(spirv_cross::SPIRType::BaseType _baseType, UInt32 _componentCount);


		//ShaderCompile
		String GetShaderEntryPointName(ShaderType _type);
		WideString GetShaderEntryPointNameW(ShaderType _type);
		String GetShaderTargetName(ShaderType _type, String _version);
		WideString GetShaderTargetNameW(ShaderType _type, WideString _version);

		//ShaderType, ShaderStage
		GLenum GetGLShaderStage(ShaderType _type);
		GLenum GetGLShaderType(ShaderType _type);
		D3D12_SHADER_VISIBILITY GetDX12ShaderVisibility(ShaderType _type);
		vk::ShaderStageFlagBits GetVKShaderStage(ShaderType _type);

		UInt32 ConvertToD3D11BindFlags(RenderBindFlags _flags);
		D3D12_RESOURCE_FLAGS ConvertToD3D12BindFlags(RenderBindFlags flags);
		D3D12_RESOURCE_STATES ConvertToD3D12ResourceStates(RenderBindFlags flags);

		vk::ImageUsageFlags ConvertToVkImageUsageFlags(RenderBindFlags usageFlags);

		String GetVendor(UInt32 _vendorCode);
	}
}