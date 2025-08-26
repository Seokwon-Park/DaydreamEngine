#pragma once

#include "Daydream/Enum/RendererEnums.h"

namespace Daydream::GraphicsUtility::DirectX
{
	DXGI_FORMAT ShaderDataTypeToDXGIFormat(ShaderDataType type);
	DXGI_FORMAT ConvertRenderFormatToDXGIFormat(RenderFormat _format);

}