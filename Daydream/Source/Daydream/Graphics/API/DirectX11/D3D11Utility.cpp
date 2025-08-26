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

	D3D11_CULL_MODE ConvertToD3D11CullMode(const CullMode& _cullmode)
	{
		switch (_cullmode)
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

	D3D11_FILL_MODE ConvertToD3D11FillMode(const FillMode& _cullmode)
	{
		switch (_cullmode)
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

	D3D11_RASTERIZER_DESC TranslateToD3D11RasterizerDesc(const RasterizerStateDesc& _desc)
	{
		D3D11_RASTERIZER_DESC desc{};
		desc.FrontCounterClockwise = _desc.frontCounterClockwise;
		desc.CullMode = ConvertToD3D11CullMode(_desc.cullMode);
		desc.FillMode = ConvertToD3D11FillMode(_desc.fillMode);

		return desc;
	}

}