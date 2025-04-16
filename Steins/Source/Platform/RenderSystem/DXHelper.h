#pragma once

#include "Steins/Enum/RendererEnums.h"

namespace Steins
{
	class DXHelper
	{
	public:
		static DXGI_FORMAT RenderFormatToDXGIFormat(RenderFormat _format);

		static std::string GetVendor(int _vendorCode);

	};
	

}