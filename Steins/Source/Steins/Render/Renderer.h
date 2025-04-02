#pragma once
#include "Steins/Render/RendererAPI.h"

namespace Steins
{
	class Renderer
	{
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}


