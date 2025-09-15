#include "DaydreamPCH.h"
#include "OpenGLUtility.h"

namespace Daydream::GraphicsUtility::OpenGL
{
	GLenum ConvertToGLShaderType(ShaderType _type)
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


	GLenum ConvertToShaderStageBit(ShaderType _type)
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
			DAYDREAM_CORE_ERROR("Unsupported or unknown render format: {0}", static_cast<int>(_format));
			return GL_RGBA8; // 기본값
		}
	}

	UInt32 ConvertRenderFormatToGLDataFormat(RenderFormat _format)
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
			DAYDREAM_CORE_ERROR("Unsupported or unknown render format for data format: {0}", static_cast<int>(_format));
			return GL_RGBA;
		}
	}

	UInt32 ConvertRenderFormatToGLDataType(RenderFormat _format)
	{
		switch (_format) {
			// 32-bit float formats
		case RenderFormat::R32G32B32A32_FLOAT:
		case RenderFormat::R32G32B32_FLOAT:
		case RenderFormat::R32G32_FLOAT:
		case RenderFormat::R32_FLOAT:
			return GL_FLOAT;

			// 16-bit float formats (half-float)
		case RenderFormat::R16G16B16A16_FLOAT:
		case RenderFormat::R16G16_FLOAT:
		case RenderFormat::R16_FLOAT:
			return GL_HALF_FLOAT;

			// 8-bit normalized/integer formats
		case RenderFormat::R8G8B8A8_UNORM:
		case RenderFormat::R8G8B8A8_UNORM_SRGB:
		case RenderFormat::B8G8R8A8_UNORM:
		case RenderFormat::B8G8R8A8_UNORM_SRGB:
		case RenderFormat::R8G8_UNORM:
		case RenderFormat::R8_UNORM:
		case RenderFormat::R8G8B8A8_UINT:
			return GL_UNSIGNED_BYTE;
		case RenderFormat::R8G8B8A8_SINT:
			return GL_BYTE;

			// 16-bit normalized/integer formats
		case RenderFormat::R16G16B16A16_UNORM:
		case RenderFormat::R16G16_UNORM:
		case RenderFormat::R16_UNORM:
		case RenderFormat::R16G16B16A16_UINT:
			return GL_UNSIGNED_SHORT;
		case RenderFormat::R16G16B16A16_SINT:
			return GL_SHORT;

			// 32-bit integer formats
		case RenderFormat::R32G32B32A32_UINT:
			return GL_UNSIGNED_INT;
		case RenderFormat::R32G32B32A32_SINT:
			return GL_INT;

			// Special packed formats
		case RenderFormat::R11G11B10_FLOAT:
			return GL_UNSIGNED_INT_10F_11F_11F_REV;
		case RenderFormat::R10G10B10A2_UNORM:
		case RenderFormat::R10G10B10A2_UINT:
			return GL_UNSIGNED_INT_2_10_10_10_REV;

			// Depth/stencil formats
		case RenderFormat::D32_FLOAT:
			return GL_FLOAT;
		case RenderFormat::D16_UNORM:
			return GL_UNSIGNED_SHORT;
		case RenderFormat::D24_UNORM_S8_UINT:
			return GL_UNSIGNED_INT_24_8;
		case RenderFormat::D32_FLOAT_S8X24_UINT:
			return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;

		case RenderFormat::BC7_UNORM:
		case RenderFormat::BC7_UNORM_SRGB:

		case RenderFormat::UNKNOWN:
		default:
			DAYDREAM_CORE_ERROR("Unsupported or unknown render format for GL Data Type conversion: {0}", static_cast<int>(_format));
			return GL_UNSIGNED_BYTE;
		}
	}

	GLint ConvertToGLFilter(FilterMode _filterMode)
	{
		switch (_filterMode)
		{
		case FilterMode::Nearest:
			return GL_NEAREST;
		case FilterMode::Linear:
			return GL_LINEAR;
		case FilterMode::NearestMipmapNearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		case FilterMode::LinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case FilterMode::NearestMipmapLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case FilterMode::LinearMipmapLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
		default:
			return GL_LINEAR; // Default fallback
		}
	}

	GLint ConvertToGLWrapMode(WrapMode _wrapMode)
	{
		switch (_wrapMode)
		{
		case WrapMode::Repeat:
			return GL_REPEAT;
		case WrapMode::MirrorRepeat:
			return GL_MIRRORED_REPEAT;
		case WrapMode::ClampToEdge:
			return GL_CLAMP_TO_EDGE;
		case WrapMode::ClampToBorder:
			return GL_CLAMP_TO_BORDER;
		default:
			return GL_REPEAT; // Default fallback
		}
	}


}

