#pragma once
#include "Steins/Enum/RendererAPI.h"

namespace Steins
{
	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return rendererAPI; }
	private:
		static RendererAPI rendererAPI;
	};
}


