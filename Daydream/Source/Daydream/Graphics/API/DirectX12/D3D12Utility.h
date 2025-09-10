#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/API/DirectX/D3DUtility.h"
#include "Daydream/Graphics/Utility/GraphicsUtility.h"

#include "Daydream/Graphics/Resources/Sampler.h"
#include "Daydream/Graphics/States/RasterizerState.h"

namespace Daydream::GraphicsUtility::DirectX12
{
	D3D12_SHADER_VISIBILITY GetDX12ShaderVisibility(ShaderType _type);
	D3D12_RESOURCE_FLAGS ConvertToD3D12BindFlags(RenderBindFlags _flags);
	D3D12_RESOURCE_STATES ConvertToD3D12ResourceStates(RenderBindFlags _flags);

	D3D12_TEXTURE_ADDRESS_MODE ConvertToD3D12WrapMode(WrapMode _wrapMode);
	D3D12_FILTER ConvertToD3D12Filter(FilterMode _minFilter, FilterMode _magFilter, FilterMode _mipFilter);
	D3D12_COMPARISON_FUNC ConvertToD3D12ComparisonFunc(ComparisonFunc _func);

	D3D12_SAMPLER_DESC TranslateToD3D12SamplerDesc(const SamplerDesc& _desc);

	constexpr D3D12_CULL_MODE ConvertToD3D12CullMode(const CullMode& _cullMode);
	constexpr D3D12_FILL_MODE ConvertToD3D12FillMode(const FillMode& _fillMode);

	D3D12_RASTERIZER_DESC TranslateToD3D12RasterizerDesc(const RasterizerStateDesc& _desc);
}