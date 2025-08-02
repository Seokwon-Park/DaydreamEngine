#include "DaydreamPCH.h"
#include "GraphicsUtil.h"

namespace Daydream
{
	namespace GraphicsUtil
	{
		UInt32 GraphicsUtil::GetRenderFormatSize(RenderFormat _format)
		{
			switch (_format)
			{
				// 128-bit formats (16 bytes)
			case RenderFormat::R32G32B32A32_FLOAT:
			case RenderFormat::R32G32B32A32_UINT:
			case RenderFormat::R32G32B32A32_SINT:
				return 16;

				// 96-bit formats (12 bytes)
			case RenderFormat::R32G32B32_FLOAT:
			case RenderFormat::R32G32B32_UINT:
			case RenderFormat::R32G32B32_SINT:
				return 12;

				// 64-bit formats (8 bytes)
			case RenderFormat::R16G16B16A16_FLOAT:
			case RenderFormat::R16G16B16A16_UNORM:
			case RenderFormat::R16G16B16A16_UINT:
			case RenderFormat::R16G16B16A16_SINT:
			case RenderFormat::R32G32_FLOAT:
			case RenderFormat::R32G32_UINT:
			case RenderFormat::R32G32_SINT:
			case RenderFormat::D32_FLOAT_S8X24_UINT:
				return 8;

				// 32-bit formats (4 bytes)
			case RenderFormat::R10G10B10A2_UNORM:
			case RenderFormat::R10G10B10A2_UINT:
			case RenderFormat::R11G11B10_FLOAT:
			case RenderFormat::R8G8B8A8_UNORM:
			case RenderFormat::R8G8B8A8_UNORM_SRGB:
			case RenderFormat::R8G8B8A8_UINT:
			case RenderFormat::R8G8B8A8_SINT:
			case RenderFormat::R16G16_FLOAT:
			case RenderFormat::R16G16_UNORM:
			case RenderFormat::R16G16_UINT:
			case RenderFormat::R16G16_SINT:
			case RenderFormat::D32_FLOAT:
			case RenderFormat::R32_FLOAT:
			case RenderFormat::R32_UINT:
			case RenderFormat::R32_SINT:
			case RenderFormat::D24_UNORM_S8_UINT:
			case RenderFormat::B8G8R8A8_UNORM:
			case RenderFormat::B8G8R8A8_UNORM_SRGB:
			case RenderFormat::B8G8R8X8_UNORM:
			case RenderFormat::R9G9B9E5_SHAREDEXP:
				return 4;

				// 16-bit formats (2 bytes)
			case RenderFormat::R8G8_UNORM:
			case RenderFormat::R8G8_UINT:
			case RenderFormat::R8G8_SINT:
			case RenderFormat::R16_FLOAT:
			case RenderFormat::D16_UNORM:
			case RenderFormat::R16_UNORM:
			case RenderFormat::R16_UINT:
			case RenderFormat::R16_SINT:
			case RenderFormat::B5G6R5_UNORM:
			case RenderFormat::B5G5R5A1_UNORM:
			case RenderFormat::B4G4R4A4_UNORM:
			case RenderFormat::A4B4G4R4_UNORM:
				return 2;

				// 8-bit formats (1 byte)
			case RenderFormat::R8_UNORM:
			case RenderFormat::R8_UINT:
			case RenderFormat::R8_SINT:
			case RenderFormat::A8_UNORM:
				return 1;

				// Compressed formats (variable size, but commonly used block sizes)
			case RenderFormat::BC1_UNORM:
			case RenderFormat::BC1_UNORM_SRGB:
			case RenderFormat::BC4_UNORM:
			case RenderFormat::BC4_SNORM:
				return 8; // 8 bytes per 4x4 block

			case RenderFormat::BC2_UNORM:
			case RenderFormat::BC2_UNORM_SRGB:
			case RenderFormat::BC3_UNORM:
			case RenderFormat::BC3_UNORM_SRGB:
			case RenderFormat::BC5_UNORM:
			case RenderFormat::BC5_SNORM:
			case RenderFormat::BC6H_UF16:
			case RenderFormat::BC6H_SF16:
			case RenderFormat::BC7_UNORM:
			case RenderFormat::BC7_UNORM_SRGB:
				return 16; // 16 bytes per 4x4 block

				// Special/YUV formats (commonly used sizes)
			case RenderFormat::NV12:
				return 1; // Variable, but 12 bits per pixel average

			case RenderFormat::YUY2:
				return 2; // 16 bits per pixel

				// Unknown or unsupported format
			case RenderFormat::UNKNOWN:
			default:
				return 0;
			}
		}
		DXGI_FORMAT GraphicsUtil::ShaderDataTypeToDXGIFormat(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float:    return DXGI_FORMAT_R32_FLOAT;
			case ShaderDataType::Float2:   return DXGI_FORMAT_R32G32_FLOAT;
			case ShaderDataType::Float3:   return DXGI_FORMAT_R32G32B32_FLOAT;
			case ShaderDataType::Float4:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case ShaderDataType::Int:      return DXGI_FORMAT_R32_SINT;
			case ShaderDataType::Int2:     return DXGI_FORMAT_R32G32_SINT;
			case ShaderDataType::Int3:     return DXGI_FORMAT_R32G32B32_SINT;
			case ShaderDataType::Int4:     return DXGI_FORMAT_R32G32B32A32_SINT;
			case ShaderDataType::Mat3x3:     return DXGI_FORMAT_UNKNOWN;
			case ShaderDataType::Mat4x4:     return DXGI_FORMAT_UNKNOWN;
			case ShaderDataType::Bool:     return DXGI_FORMAT_UNKNOWN;
			}

			Daydream_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return DXGI_FORMAT_UNKNOWN;
		}
		DXGI_FORMAT GraphicsUtil::ConvertRenderFormatToDXGIFormat(RenderFormat _format)
		{
			switch (_format)
			{
			case RenderFormat::UNKNOWN:                             return DXGI_FORMAT_UNKNOWN;
			case RenderFormat::R32G32B32A32_TYPELESS:               return DXGI_FORMAT_R32G32B32A32_TYPELESS;
			case RenderFormat::R32G32B32A32_FLOAT:                  return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case RenderFormat::R32G32B32A32_UINT:                   return DXGI_FORMAT_R32G32B32A32_UINT;
			case RenderFormat::R32G32B32A32_SINT:                   return DXGI_FORMAT_R32G32B32A32_SINT;
			case RenderFormat::R32G32B32_TYPELESS:                  return DXGI_FORMAT_R32G32B32_TYPELESS;
			case RenderFormat::R32G32B32_FLOAT:                     return DXGI_FORMAT_R32G32B32_FLOAT;
			case RenderFormat::R32G32B32_UINT:                      return DXGI_FORMAT_R32G32B32_UINT;
			case RenderFormat::R32G32B32_SINT:                      return DXGI_FORMAT_R32G32B32_SINT;
			case RenderFormat::R16G16B16A16_TYPELESS:               return DXGI_FORMAT_R16G16B16A16_TYPELESS;
			case RenderFormat::R16G16B16A16_FLOAT:                  return DXGI_FORMAT_R16G16B16A16_FLOAT;
			case RenderFormat::R16G16B16A16_UNORM:                  return DXGI_FORMAT_R16G16B16A16_UNORM;
			case RenderFormat::R16G16B16A16_UINT:                   return DXGI_FORMAT_R16G16B16A16_UINT;
			case RenderFormat::R16G16B16A16_SNORM:                  return DXGI_FORMAT_R16G16B16A16_SNORM;
			case RenderFormat::R16G16B16A16_SINT:                   return DXGI_FORMAT_R16G16B16A16_SINT;
			case RenderFormat::R32G32_TYPELESS:                     return DXGI_FORMAT_R32G32_TYPELESS;
			case RenderFormat::R32G32_FLOAT:                        return DXGI_FORMAT_R32G32_FLOAT;
			case RenderFormat::R32G32_UINT:                         return DXGI_FORMAT_R32G32_UINT;
			case RenderFormat::R32G32_SINT:                         return DXGI_FORMAT_R32G32_SINT;
			case RenderFormat::R32G8X24_TYPELESS:                   return DXGI_FORMAT_R32G8X24_TYPELESS;
			case RenderFormat::D32_FLOAT_S8X24_UINT:                return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			case RenderFormat::R32_FLOAT_X8X24_TYPELESS:            return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
			case RenderFormat::X32_TYPELESS_G8X24_UINT:             return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
			case RenderFormat::R10G10B10A2_TYPELESS:                return DXGI_FORMAT_R10G10B10A2_TYPELESS;
			case RenderFormat::R10G10B10A2_UNORM:                   return DXGI_FORMAT_R10G10B10A2_UNORM;
			case RenderFormat::R10G10B10A2_UINT:                    return DXGI_FORMAT_R10G10B10A2_UINT;
			case RenderFormat::R11G11B10_FLOAT:                     return DXGI_FORMAT_R11G11B10_FLOAT;
			case RenderFormat::R8G8B8A8_TYPELESS:                   return DXGI_FORMAT_R8G8B8A8_TYPELESS;
			case RenderFormat::R8G8B8A8_UNORM:                      return DXGI_FORMAT_R8G8B8A8_UNORM;
			case RenderFormat::R8G8B8A8_UNORM_SRGB:                 return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			case RenderFormat::R8G8B8A8_UINT:                       return DXGI_FORMAT_R8G8B8A8_UINT;
			case RenderFormat::R8G8B8A8_SNORM:                      return DXGI_FORMAT_R8G8B8A8_SNORM;
			case RenderFormat::R8G8B8A8_SINT:                       return DXGI_FORMAT_R8G8B8A8_SINT;
			case RenderFormat::R16G16_TYPELESS:                     return DXGI_FORMAT_R16G16_TYPELESS;
			case RenderFormat::R16G16_FLOAT:                        return DXGI_FORMAT_R16G16_FLOAT;
			case RenderFormat::R16G16_UNORM:                        return DXGI_FORMAT_R16G16_UNORM;
			case RenderFormat::R16G16_UINT:                         return DXGI_FORMAT_R16G16_UINT;
			case RenderFormat::R16G16_SNORM:                        return DXGI_FORMAT_R16G16_SNORM;
			case RenderFormat::R16G16_SINT:                         return DXGI_FORMAT_R16G16_SINT;
			case RenderFormat::R32_TYPELESS:                        return DXGI_FORMAT_R32_TYPELESS;
			case RenderFormat::D32_FLOAT:                           return DXGI_FORMAT_D32_FLOAT;
			case RenderFormat::R32_FLOAT:                           return DXGI_FORMAT_R32_FLOAT;
			case RenderFormat::R32_UINT:                            return DXGI_FORMAT_R32_UINT;
			case RenderFormat::R32_SINT:                            return DXGI_FORMAT_R32_SINT;
			case RenderFormat::R24G8_TYPELESS:                      return DXGI_FORMAT_R24G8_TYPELESS;
			case RenderFormat::D24_UNORM_S8_UINT:                   return DXGI_FORMAT_D24_UNORM_S8_UINT;
			case RenderFormat::R24_UNORM_X8_TYPELESS:               return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			case RenderFormat::X24_TYPELESS_G8_UINT:                return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
			case RenderFormat::R8G8_TYPELESS:                       return DXGI_FORMAT_R8G8_TYPELESS;
			case RenderFormat::R8G8_UNORM:                          return DXGI_FORMAT_R8G8_UNORM;
			case RenderFormat::R8G8_UINT:                           return DXGI_FORMAT_R8G8_UINT;
			case RenderFormat::R8G8_SNORM:                          return DXGI_FORMAT_R8G8_SNORM;
			case RenderFormat::R8G8_SINT:                           return DXGI_FORMAT_R8G8_SINT;
			case RenderFormat::R16_TYPELESS:                        return DXGI_FORMAT_R16_TYPELESS;
			case RenderFormat::R16_FLOAT:                           return DXGI_FORMAT_R16_FLOAT;
			case RenderFormat::D16_UNORM:                           return DXGI_FORMAT_D16_UNORM;
			case RenderFormat::R16_UNORM:                           return DXGI_FORMAT_R16_UNORM;
			case RenderFormat::R16_UINT:                            return DXGI_FORMAT_R16_UINT;
			case RenderFormat::R16_SNORM:                           return DXGI_FORMAT_R16_SNORM;
			case RenderFormat::R16_SINT:                            return DXGI_FORMAT_R16_SINT;
			case RenderFormat::R8_TYPELESS:                         return DXGI_FORMAT_R8_TYPELESS;
			case RenderFormat::R8_UNORM:                            return DXGI_FORMAT_R8_UNORM;
			case RenderFormat::R8_UINT:                             return DXGI_FORMAT_R8_UINT;
			case RenderFormat::R8_SNORM:                            return DXGI_FORMAT_R8_SNORM;
			case RenderFormat::R8_SINT:                             return DXGI_FORMAT_R8_SINT;
			case RenderFormat::A8_UNORM:                            return DXGI_FORMAT_A8_UNORM;
			case RenderFormat::R1_UNORM:                            return DXGI_FORMAT_R1_UNORM;
			case RenderFormat::R9G9B9E5_SHAREDEXP:                  return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
			case RenderFormat::R8G8_B8G8_UNORM:                     return DXGI_FORMAT_R8G8_B8G8_UNORM;
			case RenderFormat::G8R8_G8B8_UNORM:                     return DXGI_FORMAT_G8R8_G8B8_UNORM;
			case RenderFormat::BC1_TYPELESS:                        return DXGI_FORMAT_BC1_TYPELESS;
			case RenderFormat::BC1_UNORM:                           return DXGI_FORMAT_BC1_UNORM;
			case RenderFormat::BC1_UNORM_SRGB:                      return DXGI_FORMAT_BC1_UNORM_SRGB;
			case RenderFormat::BC2_TYPELESS:                        return DXGI_FORMAT_BC2_TYPELESS;
			case RenderFormat::BC2_UNORM:                           return DXGI_FORMAT_BC2_UNORM;
			case RenderFormat::BC2_UNORM_SRGB:                      return DXGI_FORMAT_BC2_UNORM_SRGB;
			case RenderFormat::BC3_TYPELESS:                        return DXGI_FORMAT_BC3_TYPELESS;
			case RenderFormat::BC3_UNORM:                           return DXGI_FORMAT_BC3_UNORM;
			case RenderFormat::BC3_UNORM_SRGB:                      return DXGI_FORMAT_BC3_UNORM_SRGB;
			case RenderFormat::BC4_TYPELESS:                        return DXGI_FORMAT_BC4_TYPELESS;
			case RenderFormat::BC4_UNORM:                           return DXGI_FORMAT_BC4_UNORM;
			case RenderFormat::BC4_SNORM:                           return DXGI_FORMAT_BC4_SNORM;
			case RenderFormat::BC5_TYPELESS:                        return DXGI_FORMAT_BC5_TYPELESS;
			case RenderFormat::BC5_UNORM:                           return DXGI_FORMAT_BC5_UNORM;
			case RenderFormat::BC5_SNORM:                           return DXGI_FORMAT_BC5_SNORM;
			case RenderFormat::B5G6R5_UNORM:                        return DXGI_FORMAT_B5G6R5_UNORM;
			case RenderFormat::B5G5R5A1_UNORM:                      return DXGI_FORMAT_B5G5R5A1_UNORM;
			case RenderFormat::B8G8R8A8_UNORM:                      return DXGI_FORMAT_B8G8R8A8_UNORM;
			case RenderFormat::B8G8R8X8_UNORM:                      return DXGI_FORMAT_B8G8R8X8_UNORM;
			case RenderFormat::R10G10B10_XR_BIAS_A2_UNORM:          return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
			case RenderFormat::B8G8R8A8_TYPELESS:                   return DXGI_FORMAT_B8G8R8A8_TYPELESS;
			case RenderFormat::B8G8R8A8_UNORM_SRGB:                 return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
			case RenderFormat::B8G8R8X8_TYPELESS:                   return DXGI_FORMAT_B8G8R8X8_TYPELESS;
			case RenderFormat::B8G8R8X8_UNORM_SRGB:                 return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
			case RenderFormat::BC6H_TYPELESS:                       return DXGI_FORMAT_BC6H_TYPELESS;
			case RenderFormat::BC6H_UF16:                           return DXGI_FORMAT_BC6H_UF16;
			case RenderFormat::BC6H_SF16:                           return DXGI_FORMAT_BC6H_SF16;
			case RenderFormat::BC7_TYPELESS:                        return DXGI_FORMAT_BC7_TYPELESS;
			case RenderFormat::BC7_UNORM:                           return DXGI_FORMAT_BC7_UNORM;
			case RenderFormat::BC7_UNORM_SRGB:                      return DXGI_FORMAT_BC7_UNORM_SRGB;
			case RenderFormat::AYUV:                                return DXGI_FORMAT_AYUV;
			case RenderFormat::Y410:                                return DXGI_FORMAT_Y410;
			case RenderFormat::Y416:                                return DXGI_FORMAT_Y416;
			case RenderFormat::NV12:                                return DXGI_FORMAT_NV12;
			case RenderFormat::P010:                                return DXGI_FORMAT_P010;
			case RenderFormat::P016:                                return DXGI_FORMAT_P016;
			case RenderFormat::YUY2:                                return DXGI_FORMAT_YUY2;
			case RenderFormat::Y210:                                return DXGI_FORMAT_Y210;
			case RenderFormat::Y216:                                return DXGI_FORMAT_Y216;
			case RenderFormat::NV11:                                return DXGI_FORMAT_NV11;
			case RenderFormat::AI44:                                return DXGI_FORMAT_AI44;
			case RenderFormat::IA44:                                return DXGI_FORMAT_IA44;
			case RenderFormat::P8:                                  return DXGI_FORMAT_P8;
			case RenderFormat::A8P8:                                return DXGI_FORMAT_A8P8;
			case RenderFormat::B4G4R4A4_UNORM:                      return DXGI_FORMAT_B4G4R4A4_UNORM;
			case RenderFormat::P208:                                return DXGI_FORMAT_P208;
			case RenderFormat::V208:                                return DXGI_FORMAT_V208;
			case RenderFormat::V408:                                return DXGI_FORMAT_V408;
			case RenderFormat::SAMPLER_FEEDBACK_MIN_MIP_OPAQUE:    return DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE;
			case RenderFormat::SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE: return DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE;
			default:                                                        return DXGI_FORMAT_UNKNOWN;
			}
		}
		VkFormat GraphicsUtil::ConvertRenderFormatToVkFormat(RenderFormat _format)
		{
			switch (_format)
			{
			case RenderFormat::UNKNOWN:							return VK_FORMAT_UNDEFINED;
			case RenderFormat::R32G32B32A32_TYPELESS:			return VK_FORMAT_UNDEFINED;
			case RenderFormat::R32G32B32A32_FLOAT:				return VK_FORMAT_R32G32B32A32_SFLOAT;
			case RenderFormat::R32G32B32A32_UINT:				return VK_FORMAT_R32G32B32A32_UINT;
			case RenderFormat::R32G32B32A32_SINT:				return VK_FORMAT_R32G32B32A32_SINT;
			case RenderFormat::R32G32B32_TYPELESS:				return VK_FORMAT_UNDEFINED;
			case RenderFormat::R32G32B32_FLOAT:					return VK_FORMAT_R32G32B32_SFLOAT;
			case RenderFormat::R32G32B32_UINT:					return VK_FORMAT_R32G32B32_UINT;
			case RenderFormat::R32G32B32_SINT:					return VK_FORMAT_R32G32B32_SINT;
			case RenderFormat::R16G16B16A16_TYPELESS:			return VK_FORMAT_UNDEFINED;
			case RenderFormat::R16G16B16A16_FLOAT:				return VK_FORMAT_R16G16B16A16_SFLOAT;
			case RenderFormat::R16G16B16A16_UNORM:				return VK_FORMAT_R16G16B16A16_UNORM;
			case RenderFormat::R16G16B16A16_UINT:				return VK_FORMAT_R16G16B16A16_UINT;
			case RenderFormat::R16G16B16A16_SNORM:				return VK_FORMAT_R16G16B16A16_SNORM;
			case RenderFormat::R16G16B16A16_SINT:				return VK_FORMAT_R16G16B16A16_SINT;
			case RenderFormat::R32G32_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::R32G32_FLOAT:					return VK_FORMAT_R32G32_SFLOAT;
			case RenderFormat::R32G32_UINT:						return VK_FORMAT_R32G32_UINT;
			case RenderFormat::R32G32_SINT:						return VK_FORMAT_R32G32_SINT;
			case RenderFormat::R32G8X24_TYPELESS:				return VK_FORMAT_UNDEFINED;
			case RenderFormat::D32_FLOAT_S8X24_UINT:			return VK_FORMAT_D32_SFLOAT_S8_UINT;
			case RenderFormat::R32_FLOAT_X8X24_TYPELESS:		return VK_FORMAT_D32_SFLOAT_S8_UINT;
			case RenderFormat::X32_TYPELESS_G8X24_UINT:			return VK_FORMAT_D32_SFLOAT_S8_UINT;
			case RenderFormat::R10G10B10A2_TYPELESS:			return VK_FORMAT_UNDEFINED;
			case RenderFormat::R10G10B10A2_UNORM:				return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
			case RenderFormat::R10G10B10A2_UINT:				return VK_FORMAT_A2B10G10R10_UINT_PACK32;
			case RenderFormat::R11G11B10_FLOAT:					return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
			case RenderFormat::R8G8B8A8_TYPELESS:				return VK_FORMAT_UNDEFINED;
			case RenderFormat::R8G8B8A8_UNORM:					return VK_FORMAT_R8G8B8A8_UNORM;
			case RenderFormat::R8G8B8A8_UNORM_SRGB:				return VK_FORMAT_R8G8B8A8_SRGB;
			case RenderFormat::R8G8B8A8_UINT:					return VK_FORMAT_R8G8B8A8_UINT;
			case RenderFormat::R8G8B8A8_SNORM:					return VK_FORMAT_R8G8B8A8_SNORM;
			case RenderFormat::R8G8B8A8_SINT:					return VK_FORMAT_R8G8B8A8_SINT;
			case RenderFormat::R16G16_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::R16G16_FLOAT:					return VK_FORMAT_R16G16_SFLOAT;
			case RenderFormat::R16G16_UNORM:					return VK_FORMAT_R16G16_UNORM;
			case RenderFormat::R16G16_UINT:						return VK_FORMAT_R16G16_UINT;
			case RenderFormat::R16G16_SNORM:					return VK_FORMAT_R16G16_SNORM;
			case RenderFormat::R16G16_SINT:						return VK_FORMAT_R16G16_SINT;
			case RenderFormat::R32_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::D32_FLOAT:						return VK_FORMAT_D32_SFLOAT;
			case RenderFormat::R32_FLOAT:						return VK_FORMAT_R32_SFLOAT;
			case RenderFormat::R32_UINT:						return VK_FORMAT_R32_UINT;
			case RenderFormat::R32_SINT:						return VK_FORMAT_R32_SINT;
			case RenderFormat::R24G8_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::D24_UNORM_S8_UINT:				return VK_FORMAT_D24_UNORM_S8_UINT;
			case RenderFormat::R24_UNORM_X8_TYPELESS:			return VK_FORMAT_D24_UNORM_S8_UINT;
			case RenderFormat::X24_TYPELESS_G8_UINT:			return VK_FORMAT_D24_UNORM_S8_UINT;
			case RenderFormat::R8G8_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::R8G8_UNORM:						return VK_FORMAT_R8G8_UNORM;
			case RenderFormat::R8G8_UINT:						return VK_FORMAT_R8G8_UINT;
			case RenderFormat::R8G8_SNORM:						return VK_FORMAT_R8G8_SNORM;
			case RenderFormat::R8G8_SINT:						return VK_FORMAT_R8G8_SINT;
			case RenderFormat::R16_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::R16_FLOAT:						return VK_FORMAT_R16_SFLOAT;
			case RenderFormat::D16_UNORM:						return VK_FORMAT_D16_UNORM;
			case RenderFormat::R16_UNORM:						return VK_FORMAT_R16_UNORM;
			case RenderFormat::R16_UINT:						return VK_FORMAT_R16_UINT;
			case RenderFormat::R16_SNORM:						return VK_FORMAT_R16_SNORM;
			case RenderFormat::R16_SINT:						return VK_FORMAT_R16_SINT;
			case RenderFormat::R8_TYPELESS:						return VK_FORMAT_UNDEFINED;
			case RenderFormat::R8_UNORM:						return VK_FORMAT_R8_UNORM;
			case RenderFormat::R8_UINT:							return VK_FORMAT_R8_UINT;
			case RenderFormat::R8_SNORM:						return VK_FORMAT_R8_SNORM;
			case RenderFormat::R8_SINT:							return VK_FORMAT_R8_SINT;
			case RenderFormat::A8_UNORM:						return VK_FORMAT_UNDEFINED;
			case RenderFormat::R1_UNORM:						return VK_FORMAT_UNDEFINED;
			case RenderFormat::R9G9B9E5_SHAREDEXP:				return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
			case RenderFormat::R8G8_B8G8_UNORM:					return VK_FORMAT_B8G8R8G8_422_UNORM;
			case RenderFormat::G8R8_G8B8_UNORM:					return VK_FORMAT_G8B8G8R8_422_UNORM;
			case RenderFormat::BC1_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::BC1_UNORM:						return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
			case RenderFormat::BC1_UNORM_SRGB:					return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
			case RenderFormat::BC2_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::BC2_UNORM:						return VK_FORMAT_BC2_UNORM_BLOCK;
			case RenderFormat::BC2_UNORM_SRGB:					return VK_FORMAT_BC2_SRGB_BLOCK;
			case RenderFormat::BC3_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::BC3_UNORM:						return VK_FORMAT_BC3_UNORM_BLOCK;
			case RenderFormat::BC3_UNORM_SRGB:					return VK_FORMAT_BC3_SRGB_BLOCK;
			case RenderFormat::BC4_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::BC4_UNORM:						return VK_FORMAT_BC4_UNORM_BLOCK;
			case RenderFormat::BC4_SNORM:						return VK_FORMAT_BC4_SNORM_BLOCK;
			case RenderFormat::BC5_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::BC5_UNORM:						return VK_FORMAT_BC5_UNORM_BLOCK;
			case RenderFormat::BC5_SNORM:						return VK_FORMAT_BC5_SNORM_BLOCK;
			case RenderFormat::B5G6R5_UNORM:					return VK_FORMAT_B5G6R5_UNORM_PACK16;
			case RenderFormat::B5G5R5A1_UNORM:					return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
			case RenderFormat::B8G8R8A8_UNORM:					return VK_FORMAT_B8G8R8A8_UNORM;
			case RenderFormat::B8G8R8X8_UNORM:					return VK_FORMAT_B8G8R8A8_UNORM;
			case RenderFormat::R10G10B10_XR_BIAS_A2_UNORM:		return VK_FORMAT_UNDEFINED;
			case RenderFormat::B8G8R8A8_TYPELESS:				return VK_FORMAT_UNDEFINED;
			case RenderFormat::B8G8R8A8_UNORM_SRGB:				return VK_FORMAT_B8G8R8A8_SRGB;
			case RenderFormat::B8G8R8X8_TYPELESS:				return VK_FORMAT_UNDEFINED;
			case RenderFormat::B8G8R8X8_UNORM_SRGB:				return VK_FORMAT_B8G8R8A8_SRGB;
			case RenderFormat::BC6H_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::BC6H_UF16:						return VK_FORMAT_BC6H_UFLOAT_BLOCK;
			case RenderFormat::BC6H_SF16:						return VK_FORMAT_BC6H_SFLOAT_BLOCK;
			case RenderFormat::BC7_TYPELESS:					return VK_FORMAT_UNDEFINED;
			case RenderFormat::BC7_UNORM:						return VK_FORMAT_BC7_UNORM_BLOCK;
			case RenderFormat::BC7_UNORM_SRGB:					return VK_FORMAT_BC7_SRGB_BLOCK;
			case RenderFormat::AYUV:							return VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM;
			case RenderFormat::Y410:							return VK_FORMAT_UNDEFINED; // HDR YUV 포맷, Vulkan 미지원
			case RenderFormat::Y416:							return VK_FORMAT_UNDEFINED; // HDR YUV 포맷, Vulkan 미지원
			case RenderFormat::NV12:							return VK_FORMAT_G8_B8R8_2PLANE_420_UNORM;
			case RenderFormat::P010:							return VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16;
			case RenderFormat::P016:							return VK_FORMAT_G16_B16R16_2PLANE_420_UNORM;
			case RenderFormat::YUY2:							return VK_FORMAT_G8B8G8R8_422_UNORM;
			case RenderFormat::Y210:							return VK_FORMAT_UNDEFINED; // 10-bit YUV 4:2:2 포맷, Vulkan 미지원
			case RenderFormat::Y216:							return VK_FORMAT_UNDEFINED; // 16-bit YUV 4:2:2 포맷, Vulkan 미지원
			case RenderFormat::NV11:							return VK_FORMAT_UNDEFINED; // 4:1:1 포맷, Vulkan 미지원
			case RenderFormat::AI44:							return VK_FORMAT_UNDEFINED; // Legacy format, Vulkan 미지원
			case RenderFormat::IA44:							return VK_FORMAT_UNDEFINED; // Legacy format, Vulkan 미지원
			case RenderFormat::P8:								return VK_FORMAT_UNDEFINED; // 8-bit palette, Vulkan 미지원
			case RenderFormat::A8P8:							return VK_FORMAT_UNDEFINED; // Legacy format, Vulkan 미지원
			case RenderFormat::B4G4R4A4_UNORM:					return VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT;
			case RenderFormat::P208:							return VK_FORMAT_UNDEFINED; // Planar 4:2:2, Vulkan 미지원
			case RenderFormat::V208:							return VK_FORMAT_UNDEFINED; // Vendor-specific, Vulkan 미지원
			case RenderFormat::V408:							return VK_FORMAT_UNDEFINED; // Vendor-specific, Vulkan 미지원
			case RenderFormat::SAMPLER_FEEDBACK_MIN_MIP_OPAQUE: return VK_FORMAT_UNDEFINED;
			case RenderFormat::SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE: return VK_FORMAT_UNDEFINED; // Vulkan에선 Sampler Feedback 직접적으로 없음
			default: return VK_FORMAT_UNDEFINED;
			}
		}

		UInt32 ConvertRenderFormatToGLFormat(RenderFormat _format)
		{
			switch (_format) {
				// 32-bit float formats
			case RenderFormat::R32G32B32A32_FLOAT:     return GL_RGBA32F;
			case RenderFormat::R32G32B32_FLOAT:        return GL_RGB32F;
			case RenderFormat::R32G32_FLOAT:           return GL_RG32F;
			case RenderFormat::R32_FLOAT:              return GL_R32F;

				// 16-bit float formats
			case RenderFormat::R16G16B16A16_FLOAT:     return GL_RGBA16F;
			case RenderFormat::R16G16_FLOAT:           return GL_RG16F;
			case RenderFormat::R16_FLOAT:              return GL_R16F;

				// 8-bit normalized formats
			case RenderFormat::R8G8B8A8_UNORM:         return GL_RGBA8;
			case RenderFormat::R8G8B8A8_UNORM_SRGB:    return GL_SRGB8_ALPHA8;
			case RenderFormat::B8G8R8A8_UNORM:         return GL_RGBA8; // OpenGL은 BGRA 내부 포맷 없음
			case RenderFormat::B8G8R8A8_UNORM_SRGB:    return GL_SRGB8_ALPHA8;
			case RenderFormat::R8G8_UNORM:             return GL_RG8;
			case RenderFormat::R8_UNORM:               return GL_R8;

				// 16-bit normalized formats
			case RenderFormat::R16G16B16A16_UNORM:     return GL_RGBA16;
			case RenderFormat::R16G16_UNORM:           return GL_RG16;
			case RenderFormat::R16_UNORM:              return GL_R16;

				// Integer formats
			case RenderFormat::R32G32B32A32_UINT:      return GL_RGBA32UI;
			case RenderFormat::R32G32B32A32_SINT:      return GL_RGBA32I;
			case RenderFormat::R16G16B16A16_UINT:      return GL_RGBA16UI;
			case RenderFormat::R16G16B16A16_SINT:      return GL_RGBA16I;
			case RenderFormat::R8G8B8A8_UINT:          return GL_RGBA8UI;
			case RenderFormat::R8G8B8A8_SINT:          return GL_RGBA8I;

				// Special formats
			case RenderFormat::R11G11B10_FLOAT:        return GL_R11F_G11F_B10F;
			case RenderFormat::R10G10B10A2_UNORM:      return GL_RGB10_A2;
			case RenderFormat::R10G10B10A2_UINT:       return GL_RGB10_A2UI;

				// Depth formats
			case RenderFormat::D32_FLOAT:              return GL_DEPTH_COMPONENT32F;
			case RenderFormat::D24_UNORM_S8_UINT:      return GL_DEPTH24_STENCIL8;
			case RenderFormat::D16_UNORM:              return GL_DEPTH_COMPONENT16;
			case RenderFormat::D32_FLOAT_S8X24_UINT:   return GL_DEPTH32F_STENCIL8;

				// Compressed formats (주요한 것들만)
			case RenderFormat::BC7_UNORM:              return GL_COMPRESSED_RGBA_BPTC_UNORM;
			case RenderFormat::BC7_UNORM_SRGB:         return GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;

			case RenderFormat::UNKNOWN:
			default:
				Daydream_CORE_ERROR("Unsupported or unknown render format: {0}", static_cast<int>(_format));
				return GL_RGBA8; // 기본값
			}
		}

		UInt32 GraphicsUtil::ConvertRenderFormatToGLDataFormat(RenderFormat _format)
		{
			switch (_format) {
				// 32-bit float formats
			case RenderFormat::R32G32B32A32_FLOAT:     return GL_RGBA;
			case RenderFormat::R32G32B32_FLOAT:        return GL_RGB;
			case RenderFormat::R32G32_FLOAT:           return GL_RG;
			case RenderFormat::R32_FLOAT:              return GL_RED;
				// 16-bit float formats
			case RenderFormat::R16G16B16A16_FLOAT:     return GL_RGBA;
			case RenderFormat::R16G16_FLOAT:           return GL_RG;
			case RenderFormat::R16_FLOAT:              return GL_RED;
				// 8-bit normalized formats
			case RenderFormat::R8G8B8A8_UNORM:         return GL_RGBA;
			case RenderFormat::R8G8B8A8_UNORM_SRGB:    return GL_RGBA;
			case RenderFormat::B8G8R8A8_UNORM:         return GL_BGRA;
			case RenderFormat::B8G8R8A8_UNORM_SRGB:    return GL_BGRA;
			case RenderFormat::R8G8_UNORM:             return GL_RG;
			case RenderFormat::R8_UNORM:               return GL_RED;
				// 16-bit normalized formats
			case RenderFormat::R16G16B16A16_UNORM:     return GL_RGBA;
			case RenderFormat::R16G16_UNORM:           return GL_RG;
			case RenderFormat::R16_UNORM:              return GL_RED;
				// Integer formats
			case RenderFormat::R32G32B32A32_UINT:      return GL_RGBA_INTEGER;
			case RenderFormat::R32G32B32A32_SINT:      return GL_RGBA_INTEGER;
			case RenderFormat::R16G16B16A16_UINT:      return GL_RGBA_INTEGER;
			case RenderFormat::R16G16B16A16_SINT:      return GL_RGBA_INTEGER;
			case RenderFormat::R8G8B8A8_UINT:          return GL_RGBA_INTEGER;
			case RenderFormat::R8G8B8A8_SINT:          return GL_RGBA_INTEGER;
				// Special formats
			case RenderFormat::R11G11B10_FLOAT:        return GL_RGB;
			case RenderFormat::R10G10B10A2_UNORM:      return GL_RGBA;
			case RenderFormat::R10G10B10A2_UINT:       return GL_RGBA_INTEGER;
				// Depth formats
			case RenderFormat::D32_FLOAT:              return GL_DEPTH_COMPONENT;
			case RenderFormat::D24_UNORM_S8_UINT:      return GL_DEPTH_STENCIL;
			case RenderFormat::D16_UNORM:              return GL_DEPTH_COMPONENT;
			case RenderFormat::D32_FLOAT_S8X24_UINT:   return GL_DEPTH_STENCIL;
				// Compressed formats는 data format이 필요없음 (NULL 데이터와 함께 사용)
			case RenderFormat::BC7_UNORM:              return GL_RGBA;
			case RenderFormat::BC7_UNORM_SRGB:         return GL_RGBA;
			case RenderFormat::UNKNOWN:
			default:
				Daydream_CORE_ERROR("Unsupported or unknown render format for data format: {0}", static_cast<int>(_format));
				return GL_RGBA;
			}
		}

		RenderFormat GraphicsUtil::ConvertSPIRVTypeToRenderFormat(spirv_cross::SPIRType::BaseType _baseType, UInt32 _componentCount)
		{
			switch (_baseType)
			{
			case spirv_cross::SPIRType::Float:
				switch (_componentCount)
				{
				case 1: return RenderFormat::R32_FLOAT;
				case 2: return RenderFormat::R32G32_FLOAT;
				case 3: return RenderFormat::R32G32B32_FLOAT;
				case 4: return RenderFormat::R32G32B32A32_FLOAT;
				}
				break;
			case spirv_cross::SPIRType::Int:
				switch (_componentCount)
				{
				case 1: return RenderFormat::R32_SINT;
				case 2: return RenderFormat::R32G32_SINT;
				case 3: return RenderFormat::R32G32B32_SINT;
				case 4: return RenderFormat::R32G32B32A32_SINT;
				}
				break;
			case spirv_cross::SPIRType::UInt:
				switch (_componentCount)
				{
				case 1: return RenderFormat::R32_UINT;
				case 2: return RenderFormat::R32G32_UINT;
				case 3: return RenderFormat::R32G32B32_UINT;
				case 4: return RenderFormat::R32G32B32A32_UINT;
				}
				break;
			}
			return RenderFormat::UNKNOWN;
		}

		String GraphicsUtil::GetShaderEntryPointName(ShaderType _type)
		{
			switch (_type)
			{
			case ShaderType::None:
			{
				Daydream_CORE_ASSERT(false, "Invalid type");
				return nullptr;
			}
			case ShaderType::Vertex:	return "VSMain";
			case ShaderType::Hull:		return "HSMain";
			case ShaderType::Domain:	return "DSMain";
			case ShaderType::Geometry:	return "GSMain";
			case ShaderType::Pixel:		return "PSMain";
			case ShaderType::Compute:	return "CSMain";
			default:
				break;
			}
			Daydream_CORE_ASSERT(false, "Invalid type");
			return nullptr;
		}

		WideString GraphicsUtil::GetShaderEntryPointNameW(ShaderType _type)
		{
			switch (_type)
			{
			case ShaderType::None:
			{
				Daydream_CORE_ASSERT(false, "Invalid type");
				return nullptr;
			}
			case ShaderType::Vertex:	return L"VSMain";
			case ShaderType::Hull:		return L"HSMain";
			case ShaderType::Domain:	return L"DSMain";
			case ShaderType::Geometry:	return L"GSMain";
			case ShaderType::Pixel:		return L"PSMain";
			case ShaderType::Compute:	return L"CSMain";
			default:
				break;
			}
			Daydream_CORE_ASSERT(false, "Invalid type");
			return nullptr;
		}

		String GraphicsUtil::GetShaderTargetName(ShaderType _type, String _version)
		{
			switch (_type)
			{
			case ShaderType::None:
			{
				Daydream_CORE_ASSERT(false, "Invalid type");
				return nullptr;
			}
			case ShaderType::Vertex:	return "vs_" + _version;
			case ShaderType::Hull:		return "hs_" + _version;
			case ShaderType::Domain:	return "ds_" + _version;
			case ShaderType::Geometry:	return "gs_" + _version;
			case ShaderType::Pixel:		return "ps_" + _version;
			case ShaderType::Compute:	return "cs_" + _version;
			default:
				break;
			}
			Daydream_CORE_ASSERT(false, "Invalid type");
			return nullptr;
		}

		WideString GraphicsUtil::GetShaderTargetNameW(ShaderType _type, WideString _version)
		{
			switch (_type)
			{
			case ShaderType::None:
			{
				Daydream_CORE_ASSERT(false, "Invalid type");
				return nullptr;
			}
			case ShaderType::Vertex:	return L"vs_" + _version;
			case ShaderType::Hull:		return L"hs_" + _version;
			case ShaderType::Domain:	return L"ds_" + _version;
			case ShaderType::Geometry:	return L"gs_" + _version;
			case ShaderType::Pixel:		return L"ps_" + _version;
			case ShaderType::Compute:	return L"cs_" + _version;
			default:
				break;
			}
			Daydream_CORE_ASSERT(false, "Invalid type");
			return nullptr;
		}

		GLenum GraphicsUtil::GetGLShaderType(ShaderType _type)
		{
			switch (_type)
			{

			case ShaderType::Vertex:
				return GL_VERTEX_SHADER;
				break;
			case ShaderType::Hull:
				return GL_TESS_CONTROL_SHADER;
				break;
			case ShaderType::Domain:
				return GL_TESS_EVALUATION_SHADER;
				break;
			case ShaderType::Geometry:
				return GL_GEOMETRY_SHADER;
				break;
			case ShaderType::Pixel:
				return GL_FRAGMENT_SHADER;
				break;
			};
			return GL_NONE;
		}

		D3D12_SHADER_VISIBILITY GraphicsUtil::GetDX12ShaderVisibility(ShaderType _type)
		{
			switch (_type)
			{

			case ShaderType::Vertex:
				return D3D12_SHADER_VISIBILITY_VERTEX;
				break;
			case ShaderType::Hull:
				return D3D12_SHADER_VISIBILITY_HULL;
				break;
			case ShaderType::Domain:
				return D3D12_SHADER_VISIBILITY_DOMAIN;
				break;
			case ShaderType::Geometry:
				return D3D12_SHADER_VISIBILITY_GEOMETRY;
				break;
			case ShaderType::Pixel:
				return D3D12_SHADER_VISIBILITY_PIXEL;
				break;
			};
			return D3D12_SHADER_VISIBILITY_ALL;
		}

		GLenum GraphicsUtil::GetGLShaderStage(ShaderType _type)
		{
			switch (_type)
			{
			case Daydream::ShaderType::None:
				break;
			case Daydream::ShaderType::Vertex:
				return GL_VERTEX_SHADER_BIT;
				break;
			case Daydream::ShaderType::Hull:
				return GL_TESS_CONTROL_SHADER_BIT;
				break;
			case Daydream::ShaderType::Domain:
				return GL_TESS_EVALUATION_SHADER_BIT;
				break;
			case Daydream::ShaderType::Geometry:
				return GL_GEOMETRY_SHADER_BIT;
				break;
			case Daydream::ShaderType::Pixel:
				return GL_FRAGMENT_SHADER_BIT;
				break;
			case Daydream::ShaderType::Compute:
				return GL_COMPUTE_SHADER_BIT;
				break;
			default:
				break;
			}
			return GL_NONE;
		};

		VkShaderStageFlagBits GraphicsUtil::GetVKShaderStage(ShaderType _type)
		{
			switch (_type)
			{
			case ShaderType::Vertex:
				return  VK_SHADER_STAGE_VERTEX_BIT;
				break;
			case ShaderType::Hull:
				return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				break;
			case ShaderType::Domain:
				return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
				break;
			case ShaderType::Geometry:
				return VK_SHADER_STAGE_GEOMETRY_BIT;
				break;
			case ShaderType::Pixel:
				return VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			case ShaderType::Compute:
				return VK_SHADER_STAGE_COMPUTE_BIT;
				break;
			}
			return VK_SHADER_STAGE_ALL;
		};

		UInt32 GraphicsUtil::ConvertToD3D11BindFlags(RenderBindFlags _flags)
		{
			UInt32 d3d11Flags = 0;
			if (HasFlag(_flags, RenderBindFlags::ShaderResource)) {
				d3d11Flags |= D3D11_BIND_SHADER_RESOURCE;
			}
			if (HasFlag(_flags, RenderBindFlags::RenderTarget)) {
				d3d11Flags |= D3D11_BIND_RENDER_TARGET;
			}
			if (HasFlag(_flags, RenderBindFlags::DepthStencil)) {
				d3d11Flags |= D3D11_BIND_DEPTH_STENCIL;
			}
			if (HasFlag(_flags, RenderBindFlags::UnorderedAccess)) {
				d3d11Flags |= D3D11_BIND_UNORDERED_ACCESS;
			}
			if (HasFlag(_flags, RenderBindFlags::VertexBuffer)) {
				d3d11Flags |= D3D11_BIND_VERTEX_BUFFER;
			}
			if (HasFlag(_flags, RenderBindFlags::IndexBuffer)) {
				d3d11Flags |= D3D11_BIND_INDEX_BUFFER;
			}
			if (HasFlag(_flags, RenderBindFlags::ConstantBuffer)) {
				d3d11Flags |= D3D11_BIND_CONSTANT_BUFFER;
			}
			if (HasFlag(_flags, RenderBindFlags::StreamOutput)) {
				d3d11Flags |= D3D11_BIND_STREAM_OUTPUT;
			}
			return d3d11Flags;
		}

		D3D12_RESOURCE_FLAGS GraphicsUtil::ConvertToD3D12BindFlags(RenderBindFlags flags)
		{
			D3D12_RESOURCE_FLAGS d3d12Flags = D3D12_RESOURCE_FLAG_NONE;
			if (HasFlag(flags, RenderBindFlags::ShaderResource)) {
				// SRV는 기본 플래그 (NONE)
			}
			if (HasFlag(flags, RenderBindFlags::RenderTarget)) {
				d3d12Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			}
			if (HasFlag(flags, RenderBindFlags::DepthStencil)) {
				d3d12Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			}
			if (HasFlag(flags, RenderBindFlags::UnorderedAccess)) {
				d3d12Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
			}
			return d3d12Flags;
		}

		D3D12_RESOURCE_STATES ConvertToD3D12ResourceStates(RenderBindFlags flags)
		{
			if (HasFlag(flags, RenderBindFlags::RenderTarget)) return D3D12_RESOURCE_STATE_RENDER_TARGET;
			if (HasFlag(flags, RenderBindFlags::DepthStencil)) return D3D12_RESOURCE_STATE_DEPTH_WRITE;
			if (HasFlag(flags, RenderBindFlags::UnorderedAccess)) return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
			return D3D12_RESOURCE_STATE_COMMON;
		}

		VkImageUsageFlags GraphicsUtil::ConvertToVkImageUsageFlags(RenderBindFlags usageFlags)
		{
			VkImageUsageFlags vkFlags = 0;
			if (HasFlag(usageFlags, RenderBindFlags::ShaderResource)) {
				vkFlags |= VK_IMAGE_USAGE_SAMPLED_BIT;
			}
			if (HasFlag(usageFlags, RenderBindFlags::RenderTarget)) {
				vkFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			}
			if (HasFlag(usageFlags, RenderBindFlags::DepthStencil)) {
				vkFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			}
			if (HasFlag(usageFlags, RenderBindFlags::UnorderedAccess)) { // Corresponds to Storage Image in Vulkan
				vkFlags |= VK_IMAGE_USAGE_STORAGE_BIT;
			}
			// For initial data transfer
			vkFlags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT; // For copying from buffer to image
			vkFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT; // If you need to read back or generate mips
			return vkFlags;
		}



		String GraphicsUtil::GetVendor(UInt32 _vendorCode)
		{
			switch (_vendorCode)
			{
			case 0x10DE: return "NVIDIA Corporation";
			case 0x1002: return "AMD Inc.";
			case 0x8086: return "Intel";
			case 0x1414: return "Microsoft";
			}
			Daydream_CORE_ERROR("Not a valid VendorID");
			return "";
		}

		bool GraphicsUtil::HasFlag(RenderBindFlags _target, RenderBindFlags _flags) {
			return (static_cast<std::underlying_type<RenderBindFlags>::type>(_target) &
				static_cast<std::underlying_type<RenderBindFlags>::type>(_flags)) ==
				static_cast<std::underlying_type<RenderBindFlags>::type>(_flags);
		}
	}
}