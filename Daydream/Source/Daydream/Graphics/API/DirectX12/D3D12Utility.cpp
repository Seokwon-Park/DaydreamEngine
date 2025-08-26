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
}