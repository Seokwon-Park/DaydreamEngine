#include "DaydreamPCH.h"
#include "VulkanUtility.h"
#include "Daydream/Graphics/Utility/GraphicsUtility.h"

namespace Daydream::GraphicsUtility::Vulkan
{
	vk::Format ConvertRenderFormatToVkFormat(RenderFormat _format)
	{
		switch (_format)
		{
		case RenderFormat::UNKNOWN:							return vk::Format::eUndefined;
		case RenderFormat::R32G32B32A32_TYPELESS:			return vk::Format::eUndefined;
		case RenderFormat::R32G32B32A32_FLOAT:				return vk::Format::eR32G32B32A32Sfloat;
		case RenderFormat::R32G32B32A32_UINT:				return vk::Format::eR32G32B32A32Uint;
		case RenderFormat::R32G32B32A32_SINT:				return vk::Format::eR32G32B32A32Sint;
		case RenderFormat::R32G32B32_TYPELESS:				return vk::Format::eUndefined;
		case RenderFormat::R32G32B32_FLOAT:					return vk::Format::eR32G32B32Sfloat;
		case RenderFormat::R32G32B32_UINT:					return vk::Format::eR32G32B32Uint;
		case RenderFormat::R32G32B32_SINT:					return vk::Format::eR32G32B32Sint;
		case RenderFormat::R16G16B16A16_TYPELESS:			return vk::Format::eUndefined;
		case RenderFormat::R16G16B16A16_FLOAT:				return vk::Format::eR16G16B16A16Sfloat;
		case RenderFormat::R16G16B16A16_UNORM:				return vk::Format::eR16G16B16A16Unorm;
		case RenderFormat::R16G16B16A16_UINT:				return vk::Format::eR16G16B16A16Uint;
		case RenderFormat::R16G16B16A16_SNORM:				return vk::Format::eR16G16B16A16Snorm;
		case RenderFormat::R16G16B16A16_SINT:				return vk::Format::eR16G16B16A16Sint;
		case RenderFormat::R32G32_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::R32G32_FLOAT:					return vk::Format::eR32G32Sfloat;
		case RenderFormat::R32G32_UINT:						return vk::Format::eR32G32Uint;
		case RenderFormat::R32G32_SINT:						return vk::Format::eR32G32Sint;
		case RenderFormat::R32G8X24_TYPELESS:				return vk::Format::eUndefined;
		case RenderFormat::D32_FLOAT_S8X24_UINT:			return vk::Format::eD32SfloatS8Uint;
		case RenderFormat::R32_FLOAT_X8X24_TYPELESS:		return vk::Format::eD32SfloatS8Uint;
		case RenderFormat::X32_TYPELESS_G8X24_UINT:			return vk::Format::eD32SfloatS8Uint;
		case RenderFormat::R10G10B10A2_TYPELESS:			return vk::Format::eUndefined;
		case RenderFormat::R10G10B10A2_UNORM:				return vk::Format::eA2B10G10R10UnormPack32;
		case RenderFormat::R10G10B10A2_UINT:				return vk::Format::eA2B10G10R10UintPack32;
		case RenderFormat::R11G11B10_FLOAT:					return vk::Format::eB10G11R11UfloatPack32;
		case RenderFormat::R8G8B8A8_TYPELESS:				return vk::Format::eUndefined;
		case RenderFormat::R8G8B8A8_UNORM:					return vk::Format::eR8G8B8A8Unorm;
		case RenderFormat::R8G8B8A8_UNORM_SRGB:				return vk::Format::eR8G8B8A8Srgb;
		case RenderFormat::R8G8B8A8_UINT:					return vk::Format::eR8G8B8A8Uint;
		case RenderFormat::R8G8B8A8_SNORM:					return vk::Format::eR8G8B8A8Snorm;
		case RenderFormat::R8G8B8A8_SINT:					return vk::Format::eR8G8B8A8Sint;
		case RenderFormat::R16G16_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::R16G16_FLOAT:					return vk::Format::eR16G16Sfloat;
		case RenderFormat::R16G16_UNORM:					return vk::Format::eR16G16Unorm;
		case RenderFormat::R16G16_UINT:						return vk::Format::eR16G16Uint;
		case RenderFormat::R16G16_SNORM:					return vk::Format::eR16G16Snorm;
		case RenderFormat::R16G16_SINT:						return vk::Format::eR16G16Sint;
		case RenderFormat::R32_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::D32_FLOAT:						return vk::Format::eD32Sfloat;
		case RenderFormat::R32_FLOAT:						return vk::Format::eR32Sfloat;
		case RenderFormat::R32_UINT:						return vk::Format::eR32Uint;
		case RenderFormat::R32_SINT:						return vk::Format::eR32Sint;
		case RenderFormat::R24G8_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::D24_UNORM_S8_UINT:				return vk::Format::eD24UnormS8Uint;
		case RenderFormat::R24_UNORM_X8_TYPELESS:			return vk::Format::eD24UnormS8Uint;
		case RenderFormat::X24_TYPELESS_G8_UINT:			return vk::Format::eD24UnormS8Uint;
		case RenderFormat::R8G8_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::R8G8_UNORM:						return vk::Format::eR8G8Unorm;
		case RenderFormat::R8G8_UINT:						return vk::Format::eR8G8Uint;
		case RenderFormat::R8G8_SNORM:						return vk::Format::eR8G8Snorm;
		case RenderFormat::R8G8_SINT:						return vk::Format::eR8G8Sint;
		case RenderFormat::R16_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::R16_FLOAT:						return vk::Format::eR16Sfloat;
		case RenderFormat::D16_UNORM:						return vk::Format::eD16Unorm;
		case RenderFormat::R16_UNORM:						return vk::Format::eR16Unorm;
		case RenderFormat::R16_UINT:						return vk::Format::eR16Uint;
		case RenderFormat::R16_SNORM:						return vk::Format::eR16Snorm;
		case RenderFormat::R16_SINT:						return vk::Format::eR16Sint;
		case RenderFormat::R8_TYPELESS:						return vk::Format::eUndefined;
		case RenderFormat::R8_UNORM:						return vk::Format::eR8Unorm;
		case RenderFormat::R8_UINT:							return vk::Format::eR8Uint;
		case RenderFormat::R8_SNORM:						return vk::Format::eR8Snorm;
		case RenderFormat::R8_SINT:							return vk::Format::eR8Sint;
		case RenderFormat::A8_UNORM:						return vk::Format::eUndefined;
		case RenderFormat::R1_UNORM:						return vk::Format::eUndefined;
		case RenderFormat::R9G9B9E5_SHAREDEXP:				return vk::Format::eE5B9G9R9UfloatPack32;
		case RenderFormat::R8G8_B8G8_UNORM:					return vk::Format::eB8G8R8G8422Unorm;
		case RenderFormat::G8R8_G8B8_UNORM:					return vk::Format::eG8B8G8R8422Unorm;
		case RenderFormat::BC1_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::BC1_UNORM:						return vk::Format::eBc1RgbaUnormBlock;
		case RenderFormat::BC1_UNORM_SRGB:					return vk::Format::eBc1RgbaSrgbBlock;
		case RenderFormat::BC2_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::BC2_UNORM:						return vk::Format::eBc2UnormBlock;
		case RenderFormat::BC2_UNORM_SRGB:					return vk::Format::eBc2SrgbBlock;
		case RenderFormat::BC3_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::BC3_UNORM:						return vk::Format::eBc3UnormBlock;
		case RenderFormat::BC3_UNORM_SRGB:					return vk::Format::eBc3SrgbBlock;
		case RenderFormat::BC4_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::BC4_UNORM:						return vk::Format::eBc4UnormBlock;
		case RenderFormat::BC4_SNORM:						return vk::Format::eBc4SnormBlock;
		case RenderFormat::BC5_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::BC5_UNORM:						return vk::Format::eBc5UnormBlock;
		case RenderFormat::BC5_SNORM:						return vk::Format::eBc5SnormBlock;
		case RenderFormat::B5G6R5_UNORM:					return vk::Format::eB5G6R5UnormPack16;
		case RenderFormat::B5G5R5A1_UNORM:					return vk::Format::eB5G5R5A1UnormPack16;
		case RenderFormat::B8G8R8A8_UNORM:					return vk::Format::eB8G8R8A8Unorm;
		case RenderFormat::B8G8R8X8_UNORM:					return vk::Format::eB8G8R8A8Unorm;
			// Note: R10G10B10_XR_BIAS_A2_UNORM is a DirectX-specific format. No direct Vulkan equivalent.
		case RenderFormat::R10G10B10_XR_BIAS_A2_UNORM:		return vk::Format::eUndefined;
		case RenderFormat::B8G8R8A8_TYPELESS:				return vk::Format::eUndefined;
		case RenderFormat::B8G8R8A8_UNORM_SRGB:				return vk::Format::eB8G8R8A8Srgb;
		case RenderFormat::B8G8R8X8_TYPELESS:				return vk::Format::eUndefined;
		case RenderFormat::B8G8R8X8_UNORM_SRGB:				return vk::Format::eB8G8R8A8Srgb;
		case RenderFormat::BC6H_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::BC6H_UF16:						return vk::Format::eBc6HUfloatBlock;
		case RenderFormat::BC6H_SF16:						return vk::Format::eBc6HSfloatBlock;
		case RenderFormat::BC7_TYPELESS:					return vk::Format::eUndefined;
		case RenderFormat::BC7_UNORM:						return vk::Format::eBc7UnormBlock;
		case RenderFormat::BC7_UNORM_SRGB:					return vk::Format::eBc7SrgbBlock;
			// Note: AYUV is complex. This mapping might depend on specific usage.
		case RenderFormat::AYUV:							return vk::Format::eG8B8R83Plane420Unorm;
		case RenderFormat::Y410:							return vk::Format::eUndefined;
		case RenderFormat::Y416:							return vk::Format::eUndefined;
		case RenderFormat::NV12:							return vk::Format::eG8B8R82Plane420Unorm;
		case RenderFormat::P010:							return vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16;
		case RenderFormat::P016:							return vk::Format::eG16B16R162Plane420Unorm;
		case RenderFormat::YUY2:							return vk::Format::eG8B8G8R8422Unorm;
		case RenderFormat::Y210:							return vk::Format::eUndefined;
		case RenderFormat::Y216:							return vk::Format::eUndefined;
		case RenderFormat::NV11:							return vk::Format::eUndefined;
		case RenderFormat::AI44:							return vk::Format::eUndefined;
		case RenderFormat::IA44:							return vk::Format::eUndefined;
		case RenderFormat::P8:								return vk::Format::eUndefined;
		case RenderFormat::A8P8:							return vk::Format::eUndefined;
		case RenderFormat::B4G4R4A4_UNORM:					return vk::Format::eA4R4G4B4UnormPack16EXT;
		case RenderFormat::P208:							return vk::Format::eUndefined;
		case RenderFormat::V208:							return vk::Format::eUndefined;
		case RenderFormat::V408:							return vk::Format::eUndefined;
		case RenderFormat::SAMPLER_FEEDBACK_MIN_MIP_OPAQUE: return vk::Format::eUndefined;
		case RenderFormat::SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE: return vk::Format::eUndefined;
		default:
			// It's good practice to assert or log an error for unhandled cases.
			// assert(false && "Unknown RenderFormat!");
			return vk::Format::eUndefined;
		}
	}

	vk::ShaderStageFlagBits ConvertToShaderStageFlagBit(ShaderType _type)
	{
		switch (_type)
		{
		case ShaderType::Vertex:
			return vk::ShaderStageFlagBits::eVertex;
			break;
		case ShaderType::Hull:
			return vk::ShaderStageFlagBits::eTessellationControl;
			break;
		case ShaderType::Domain:
			return vk::ShaderStageFlagBits::eTessellationEvaluation;
			break;
		case ShaderType::Geometry:
			return vk::ShaderStageFlagBits::eGeometry;
			break;
		case ShaderType::Pixel:
			return vk::ShaderStageFlagBits::eFragment;
			break;
		case ShaderType::Compute:
			return vk::ShaderStageFlagBits::eCompute;
			break;
		}
		return vk::ShaderStageFlagBits::eAll;
	};

	vk::ImageUsageFlags ConvertToVkImageUsageFlags(RenderBindFlags usageFlags)
	{
		vk::ImageUsageFlags vkFlags{};
		if (HasFlag(usageFlags, RenderBindFlags::ShaderResource)) {
			vkFlags |= vk::ImageUsageFlagBits::eSampled;
		}
		if (HasFlag(usageFlags, RenderBindFlags::RenderTarget)) {
			vkFlags |= vk::ImageUsageFlagBits::eColorAttachment;
		}
		if (HasFlag(usageFlags, RenderBindFlags::DepthStencil)) {
			vkFlags |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
		}
		if (HasFlag(usageFlags, RenderBindFlags::UnorderedAccess)) { // Corresponds to Storage Image in Vulkan
			vkFlags |= vk::ImageUsageFlagBits::eStorage;
		}
		// For initial data transfer
		vkFlags |= vk::ImageUsageFlagBits::eTransferDst; // For copying from buffer to image
		vkFlags |= vk::ImageUsageFlagBits::eTransferSrc; // If you need to read back or generate mips
		return vkFlags;
	}

	constexpr vk::SamplerAddressMode ConvertToVkAddressMode(WrapMode _wrapMode)
	{
		switch (_wrapMode)
		{
		case WrapMode::Repeat:
			return vk::SamplerAddressMode::eRepeat;
		case WrapMode::ClampToEdge:
			return vk::SamplerAddressMode::eClampToEdge;
		case WrapMode::ClampToBorder:
			return vk::SamplerAddressMode::eClampToBorder;
		case WrapMode::MirrorRepeat:
			return vk::SamplerAddressMode::eMirroredRepeat;
		default:
			break;
		}
		return vk::SamplerAddressMode::eRepeat;
	}

	constexpr vk::Filter ConvertToVkFilter(FilterMode _filterMode)
	{
		switch (_filterMode)
		{
		case FilterMode::Nearest:
		case FilterMode::NearestMipmapNearest:
		case FilterMode::NearestMipmapLinear:
			return vk::Filter::eNearest;

		case FilterMode::Linear:
		case FilterMode::LinearMipmapNearest:
		case FilterMode::LinearMipmapLinear:
			return vk::Filter::eLinear;
		default:
			break;
		}
		return vk::Filter::eLinear;
	}

	constexpr vk::SamplerMipmapMode ConvertToVkMipmapMode(FilterMode _mipMapFilterMode)
	{
		switch (_mipMapFilterMode)
		{
		case FilterMode::Nearest:
		case FilterMode::NearestMipmapNearest:
		case FilterMode::LinearMipmapNearest:
			return vk::SamplerMipmapMode::eNearest;

		case FilterMode::Linear:
		case FilterMode::NearestMipmapLinear:
		case FilterMode::LinearMipmapLinear:
			return vk::SamplerMipmapMode::eLinear;
		default:
			break;
		}
		return vk::SamplerMipmapMode::eLinear;
	}

	constexpr vk::CompareOp ConvertToVkCompareOp(ComparisonFunc _func)
	{
		switch (_func)
		{
		case ComparisonFunc::Never:        return vk::CompareOp::eNever;
		case ComparisonFunc::Less:         return vk::CompareOp::eLess;
		case ComparisonFunc::Equal:        return vk::CompareOp::eEqual;
		case ComparisonFunc::LessEqual:    return vk::CompareOp::eLessOrEqual;
		case ComparisonFunc::Greater:      return vk::CompareOp::eGreater;
		case ComparisonFunc::NotEqual:     return vk::CompareOp::eNotEqual;
		case ComparisonFunc::GreaterEqual: return vk::CompareOp::eGreaterOrEqual; 
		case ComparisonFunc::Always:       return vk::CompareOp::eAlways;
		default:
			break;
		}
		return vk::CompareOp::eNever;
	}

	vk::SamplerCreateInfo TranslateToVulkanSamplerCreateInfo(const SamplerDesc& _desc)
	{
		vk::SamplerCreateInfo info{};
		
		info.magFilter = ConvertToVkFilter(_desc.magFilter);
		info.minFilter = ConvertToVkFilter(_desc.minFilter);
		info.mipmapMode = ConvertToVkMipmapMode(_desc.mipFilter);
		info.addressModeU = ConvertToVkAddressMode(_desc.wrapU);
		info.addressModeV = ConvertToVkAddressMode(_desc.wrapV);
		info.addressModeW = ConvertToVkAddressMode(_desc.wrapW);
		info.anisotropyEnable = VK_FALSE;
		info.maxAnisotropy = _desc.maxAnisotropy;
		info.borderColor = vk::BorderColor::eIntTransparentBlack;
		info.unnormalizedCoordinates = VK_FALSE;
		info.compareEnable = VK_FALSE;
		info.compareOp = ConvertToVkCompareOp(_desc.comparisonFunc);

		return info;
	}

	constexpr vk::CullModeFlags ConvertToVulkanCullMode(const CullMode& _cullMode)
	{
		switch (_cullMode)
		{
		case CullMode::None:
			return vk::CullModeFlagBits::eNone;
		case CullMode::Front:
			return vk::CullModeFlagBits::eFront;
		case CullMode::Back:
			return vk::CullModeFlagBits::eBack;
		default:
			return vk::CullModeFlagBits::eNone;
		}
		return  vk::CullModeFlagBits::eFrontAndBack;
	}

	constexpr vk::PolygonMode ConvertToVulkanFillMode(const FillMode& _fillMode)
	{
		switch (_fillMode)
		{
		case FillMode::Solid:
			return vk::PolygonMode::eFill;
		case FillMode::Wireframe:
			return vk::PolygonMode::eLine;
		default:
			return vk::PolygonMode::eFill;
		}
		return vk::PolygonMode::eFill;
	}

	vk::PipelineRasterizationStateCreateInfo TranslateToVulkanRasterizerCreateInfo(const RasterizerStateDesc& _desc)
	{
		vk::PipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = ConvertToVulkanFillMode(_desc.fillMode);
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = ConvertToVulkanCullMode(_desc.cullMode);
		rasterizer.frontFace = _desc.frontCounterClockwise ? vk::FrontFace::eCounterClockwise : vk::FrontFace::eClockwise;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
		return rasterizer;
	}

}
