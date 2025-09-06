#include "DaydreamPCH.h"
#include "D3D11Utility.h"

namespace Daydream::GraphicsUtility::DirectX11
{
	UInt32 ConvertToD3D11BindFlags(RenderBindFlags _flags)
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

	D3D11_CULL_MODE ConvertToD3D11CullMode(const CullMode& _cullMode)
	{
		switch (_cullMode)
		{
		case CullMode::None:
			return D3D11_CULL_NONE;
		case CullMode::Front:
			return D3D11_CULL_FRONT;
		case CullMode::Back:
			return D3D11_CULL_BACK;
		default:
			return D3D11_CULL_NONE;
		}
		return D3D11_CULL_NONE;
	}

	D3D11_FILL_MODE ConvertToD3D11FillMode(const FillMode& _fillMode)
	{
		switch (_fillMode)
		{
		case FillMode::Solid:
			return D3D11_FILL_SOLID;
		case FillMode::Wireframe:
			return D3D11_FILL_WIREFRAME;
		default:
			return D3D11_FILL_SOLID;
		}
		return D3D11_FILL_SOLID;
	}



	D3D11_FILTER ConvertToD3D11Filter(FilterMode _minFilter, FilterMode _magFilter, FilterMode _mipFilter)
	{
		if (_minFilter == FilterMode::Nearest && _magFilter == FilterMode::Nearest && _mipFilter == FilterMode::Nearest)
			return D3D11_FILTER_MIN_MAG_MIP_POINT;

		if (_minFilter == FilterMode::Nearest && _magFilter == FilterMode::Nearest && _mipFilter == FilterMode::Linear)
			return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;

		if (_minFilter == FilterMode::Nearest && _magFilter == FilterMode::Linear && _mipFilter == FilterMode::Nearest)
			return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;

		if (_minFilter == FilterMode::Nearest && _magFilter == FilterMode::Linear && _mipFilter == FilterMode::Linear)
			return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;

		if (_minFilter == FilterMode::Linear && _magFilter == FilterMode::Nearest && _mipFilter == FilterMode::Nearest)
			return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;

		if (_minFilter == FilterMode::Linear && _magFilter == FilterMode::Nearest && _mipFilter == FilterMode::Linear)
			return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;

		if (_minFilter == FilterMode::Linear && _magFilter == FilterMode::Linear && _mipFilter == FilterMode::Nearest)
			return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

		if (_minFilter == FilterMode::Linear && _magFilter == FilterMode::Linear && _mipFilter == FilterMode::Linear)
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}

	D3D11_TEXTURE_ADDRESS_MODE ConvertToD3D11WrapMode(WrapMode _wrapMode)
	{
		switch (_wrapMode)
		{
		case WrapMode::Repeat:
			return D3D11_TEXTURE_ADDRESS_WRAP;
			break;
		case WrapMode::ClampToEdge:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
			break;
		case WrapMode::ClampToBorder:
			return D3D11_TEXTURE_ADDRESS_BORDER;
			break;
		case WrapMode::MirrorRepeat:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
			break;
		default:
			break;
		}
		return D3D11_TEXTURE_ADDRESS_MODE();
	}

	D3D11_COMPARISON_FUNC ConvertToD3D11ComparisonFunc(ComparisonFunc _func)
	{
		switch (_func)
		{
		case ComparisonFunc::Never:
			return D3D11_COMPARISON_NEVER;
			break;
		case ComparisonFunc::Less:
			return D3D11_COMPARISON_LESS;
			break;
		case ComparisonFunc::Equal:
			return D3D11_COMPARISON_EQUAL;
			break;
		case ComparisonFunc::LessEqual:
			return D3D11_COMPARISON_LESS_EQUAL;
			break;
		case ComparisonFunc::Greater:
			return D3D11_COMPARISON_GREATER;
			break;
		case ComparisonFunc::NotEqual:
			return D3D11_COMPARISON_NOT_EQUAL;
			break;
		case ComparisonFunc::GreaterEqual:
			return D3D11_COMPARISON_GREATER_EQUAL;
			break;
		case ComparisonFunc::Always:
			return D3D11_COMPARISON_ALWAYS;
			break;
		default:
			break;
		}
	}

	D3D11_SAMPLER_DESC TranslateToD3D11SamplerDesc(const SamplerDesc& _desc)
	{
		//D3D11_SAMPLER_DESC samplerDesc;
		//ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		//samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		//samplerDesc.MinLOD = 0;
		//samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		D3D11_SAMPLER_DESC desc{};
		desc.Filter = ConvertToD3D11Filter(_desc.minFilter ,_desc.magFilter, _desc.mipFilter);
		desc.AddressU = ConvertToD3D11WrapMode(_desc.wrapU);
		desc.AddressV = ConvertToD3D11WrapMode(_desc.wrapV);
		desc.AddressW = ConvertToD3D11WrapMode(_desc.wrapW);
		desc.MipLODBias = _desc.lodBias;
		desc.MaxAnisotropy = _desc.maxAnisotropy;
		desc.ComparisonFunc = ConvertToD3D11ComparisonFunc(_desc.comparisonFunc);
		for (int i = 0; i < 4; i++)
		{
			desc.BorderColor[i] = _desc.borderColor[i];
		}
		desc.MinLOD = _desc.minLod;
		desc.MaxLOD = _desc.maxLod;
		return desc;
	}

	D3D11_RASTERIZER_DESC TranslateToD3D11RasterizerDesc(const RasterizerStateDesc& _desc)
	{
		D3D11_RASTERIZER_DESC desc{};
		desc.CullMode = ConvertToD3D11CullMode(_desc.cullMode);
		desc.FillMode = ConvertToD3D11FillMode(_desc.fillMode);
		desc.FrontCounterClockwise = _desc.frontCounterClockwise;
		desc.DepthBias = _desc.depthBias;
		desc.DepthBiasClamp = _desc.depthBiasClamp;
		desc.SlopeScaledDepthBias = _desc.slopeScaledDepthBias;
		desc.DepthClipEnable = _desc.depthClipEnable;
		desc.ScissorEnable = _desc.scissorEnable;
		desc.MultisampleEnable = _desc.multisampleEnable;
		desc.AntialiasedLineEnable = _desc.antialiasedLineEnable;

		return desc;
	}

}