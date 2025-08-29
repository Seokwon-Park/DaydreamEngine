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


	D3D12_RESOURCE_FLAGS ConvertToD3D12BindFlags(RenderBindFlags flags)
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