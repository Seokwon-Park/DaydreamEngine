#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/API/DirectX/D3DUtility.h"
#include "Daydream/Graphics/Utility/GraphicsUtility.h"

#include "Daydream/Graphics/States/RasterizerState.h"
#include "Daydream/Graphics/Resources/Sampler.h"

namespace Daydream::GraphicsUtility::DirectX11
{
	UInt32 ConvertToD3D11BindFlags(RenderBindFlags _flags);

	D3D11_TEXTURE_ADDRESS_MODE ConvertToD3D11WrapMode(WrapMode _wrapMode);
	D3D11_FILTER ConvertToD3D11Filter(FilterMode _minFilter, FilterMode _magFilter, FilterMode _mipFilter);
	D3D11_COMPARISON_FUNC ConvertToD3D11ComparisonFunc(ComparisonFunc _func);

	D3D11_SAMPLER_DESC TranslateToD3D11SamplerDesc(const SamplerDesc& _desc);

	D3D11_CULL_MODE ConvertToD3D11CullMode(const CullMode& _cullMode);
	D3D11_FILL_MODE ConvertToD3D11FillMode(const FillMode& _fillMode);

	D3D11_RASTERIZER_DESC TranslateToD3D11RasterizerDesc(const RasterizerStateDesc& _desc);

	
}