#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/API/DirectX/D3DUtility.h"
#include "Daydream/Graphics/Utility/GraphicsUtility.h"

#include "Daydream/Graphics/States/RasterizerState.h"

namespace Daydream::GraphicsUtility::DirectX11
{
	UInt32 ConvertToD3D11BindFlags(RenderBindFlags _flags);
	D3D11_CULL_MODE ConvertToD3D11CullMode(const CullMode& _cullMode);
	D3D11_FILL_MODE ConvertToD3D11FillMode(const FillMode& _fillMode);

	D3D11_RASTERIZER_DESC TranslateToD3D11RasterizerDesc(const RasterizerStateDesc& _desc);
}