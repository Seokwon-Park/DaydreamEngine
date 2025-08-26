#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/API/DirectX/D3DUtility.h"
#include "Daydream/Graphics/Utility/GraphicsUtility.h"

namespace Daydream::GraphicsUtility::DirectX12
{
	D3D12_SHADER_VISIBILITY GetDX12ShaderVisibility(ShaderType _type);
	D3D12_RESOURCE_FLAGS ConvertToD3D12BindFlags(RenderBindFlags flags);
	D3D12_RESOURCE_STATES ConvertToD3D12ResourceStates(RenderBindFlags flags);
}