#include "DaydreamPCH.h"
#include "D3D12Utility.h"

namespace Daydream::GraphicsUtility::DirectX12
{
	D3D12_SHADER_VISIBILITY GetDX12ShaderVisibility(ShaderType _type)
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


	D3D12_RESOURCE_FLAGS ConvertToD3D12BindFlags(RenderBindFlags _flags)
	{
		D3D12_RESOURCE_FLAGS d3d12Flags = D3D12_RESOURCE_FLAG_NONE;
		if (HasFlag(_flags, RenderBindFlags::ShaderResource)) {
			// SRV는 기본 플래그 (NONE)
		}
		if (HasFlag(_flags, RenderBindFlags::RenderTarget)) {
			d3d12Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		}
		if (HasFlag(_flags, RenderBindFlags::DepthStencil)) {
			d3d12Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		}
		if (HasFlag(_flags, RenderBindFlags::UnorderedAccess)) {
			d3d12Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		}
		return d3d12Flags;
	}

	D3D12_RESOURCE_STATES ConvertToD3D12ResourceStates(RenderBindFlags _flags)
	{
		if (HasFlag(_flags, RenderBindFlags::RenderTarget)) return D3D12_RESOURCE_STATE_RENDER_TARGET;
		if (HasFlag(_flags, RenderBindFlags::DepthStencil)) return D3D12_RESOURCE_STATE_DEPTH_WRITE;
		if (HasFlag(_flags, RenderBindFlags::UnorderedAccess)) return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		return D3D12_RESOURCE_STATE_COMMON;
	}
	D3D12_FILTER ConvertToD3D12Filter(FilterMode _minFilterMode, FilterMode _magFilterMode, FilterMode _mipFilterMode)
	{
		if (_minFilterMode == FilterMode::Nearest && _magFilterMode == FilterMode::Nearest && _mipFilterMode == FilterMode::Nearest)
			return D3D12_FILTER_MIN_MAG_MIP_POINT;

		if (_minFilterMode == FilterMode::Nearest && _magFilterMode == FilterMode::Nearest && _mipFilterMode == FilterMode::Linear)
			return D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;

		if (_minFilterMode == FilterMode::Nearest && _magFilterMode == FilterMode::Linear && _mipFilterMode == FilterMode::Nearest)
			return D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;

		if (_minFilterMode == FilterMode::Nearest && _magFilterMode == FilterMode::Linear && _mipFilterMode == FilterMode::Linear)
			return D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR;

		if (_minFilterMode == FilterMode::Linear && _magFilterMode == FilterMode::Nearest && _mipFilterMode == FilterMode::Nearest)
			return D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;

		if (_minFilterMode == FilterMode::Linear && _magFilterMode == FilterMode::Nearest && _mipFilterMode == FilterMode::Linear)
			return D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;

		if (_minFilterMode == FilterMode::Linear && _magFilterMode == FilterMode::Linear && _mipFilterMode == FilterMode::Nearest)
			return D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;

		if (_minFilterMode == FilterMode::Linear && _magFilterMode == FilterMode::Linear && _mipFilterMode == FilterMode::Linear)
			return D3D12_FILTER_MIN_MAG_MIP_LINEAR;

		return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	}

	D3D12_TEXTURE_ADDRESS_MODE ConvertToD3D12WrapMode(WrapMode _wrapMode)
	{
		switch (_wrapMode)
		{
		case WrapMode::Repeat:
			return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		case WrapMode::ClampToEdge:
			return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		case WrapMode::ClampToBorder:
			return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		case WrapMode::MirrorRepeat:
			return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
		default:
			break;
		}
		return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	}

	D3D12_COMPARISON_FUNC ConvertToD3D12ComparisonFunc(ComparisonFunc _func)
	{
		switch (_func)
		{
		case ComparisonFunc::Never:
			return D3D12_COMPARISON_FUNC_NEVER;
		case ComparisonFunc::Less:
			return D3D12_COMPARISON_FUNC_LESS;
		case ComparisonFunc::Equal:
			return D3D12_COMPARISON_FUNC_EQUAL;
		case ComparisonFunc::LessEqual:
			return D3D12_COMPARISON_FUNC_LESS_EQUAL;
		case ComparisonFunc::Greater:
			return D3D12_COMPARISON_FUNC_GREATER;
		case ComparisonFunc::NotEqual:
			return D3D12_COMPARISON_FUNC_NOT_EQUAL;
		case ComparisonFunc::GreaterEqual:
			return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		case ComparisonFunc::Always:
			return D3D12_COMPARISON_FUNC_ALWAYS;
		default:
			break;
		}
		return D3D12_COMPARISON_FUNC_NEVER;
	}

	D3D12_SAMPLER_DESC TranslateToD3D12SamplerDesc(const SamplerDesc& _desc)
	{
		//D3D12_SAMPLER_DESC samplerDesc;
		//ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		//samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		//samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.ComparisonFunc = D3D12_COMPARISON_NEVER;
		//samplerDesc.MinLOD = 0;
		//samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;

		D3D12_SAMPLER_DESC desc{};
		desc.Filter = ConvertToD3D12Filter(_desc.minFilter, _desc.magFilter, _desc.mipFilter);
		desc.AddressU = ConvertToD3D12WrapMode(_desc.wrapU);
		desc.AddressV = ConvertToD3D12WrapMode(_desc.wrapV);
		desc.AddressW = ConvertToD3D12WrapMode(_desc.wrapW);
		desc.MipLODBias = _desc.lodBias;
		desc.MaxAnisotropy = _desc.maxAnisotropy;
		desc.ComparisonFunc = ConvertToD3D12ComparisonFunc(_desc.comparisonFunc);
		for (int i = 0; i < 4; i++)
		{
			desc.BorderColor[i] = _desc.borderColor[i];
		}
		desc.MinLOD = _desc.minLod;
		desc.MaxLOD = _desc.maxLod;
		return desc;
	}
	constexpr D3D12_CULL_MODE ConvertToD3D12CullMode(const CullMode& _cullMode)
	{
		switch (_cullMode)
		{
		case CullMode::None:
			return D3D12_CULL_MODE_NONE;
		case CullMode::Front:
			return D3D12_CULL_MODE_FRONT;
		case CullMode::Back:
			return D3D12_CULL_MODE_BACK;
		default:
			return D3D12_CULL_MODE_NONE;
		}
		return D3D12_CULL_MODE_NONE;
	}
	constexpr D3D12_FILL_MODE ConvertToD3D12FillMode(const FillMode& _fillMode)
	{
		switch (_fillMode)
		{
		case FillMode::Solid:
			return D3D12_FILL_MODE_SOLID;
		case FillMode::Wireframe:
			return D3D12_FILL_MODE_WIREFRAME;
		default:
			return D3D12_FILL_MODE_SOLID;
		}
		return D3D12_FILL_MODE_SOLID;
	}

	D3D12_RASTERIZER_DESC TranslateToD3D12RasterizerDesc(const RasterizerStateDesc& _desc)
	{
		D3D12_RASTERIZER_DESC desc{};
		desc.FillMode = ConvertToD3D12FillMode(_desc.fillMode);
		desc.CullMode = ConvertToD3D12CullMode(_desc.cullMode);
		desc.FrontCounterClockwise = _desc.frontCounterClockwise;
		desc.DepthBias = _desc.depthBias;
		desc.DepthBiasClamp = _desc.depthBiasClamp;
		desc.SlopeScaledDepthBias = _desc.slopeScaledDepthBias;
		desc.DepthClipEnable = _desc.depthClipEnable;
		desc.MultisampleEnable = _desc.multisampleEnable;
		desc.AntialiasedLineEnable = _desc.antialiasedLineEnable;
		return desc;
	}
}