#pragma once

namespace Steins
{
	enum class RendererAPIType
	{
		None,
		OpenGL,
		DirectX11,
		DirectX12,
		Vulkan,
		Metal
	};

	enum class ShaderType
	{
		None,
		Vertex,
		Hull,
		Domain,
		Geometry,
		Pixel,
		Compute,
	};
}