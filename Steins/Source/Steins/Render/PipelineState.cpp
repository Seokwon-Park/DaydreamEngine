#include "SteinsPCH.h"
#include "PipelineState.h"

#include "Renderer.h"

namespace Steins
{
	Shared<PipelineState> PipelineState::Create(PipelineStateDesc _desc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::None:STEINS_CORE_ASSERT(false, "..."); return nullptr;
		case RendererAPIType::OpenGL:STEINS_CORE_ASSERT(false, "..."); return nullptr;
		case RendererAPIType::DirectX11:STEINS_CORE_ASSERT(false, "..."); return nullptr;
		case RendererAPIType::DirectX12:STEINS_CORE_ASSERT(false, "..."); return nullptr;
		case RendererAPIType::Vulkan:STEINS_CORE_ASSERT(false, "..."); return nullptr;
		case RendererAPIType::Metal:STEINS_CORE_ASSERT(false, "...");  return nullptr;
		default:
			break;
		}
		return nullptr;
	}
	//void PipelineState::SetShaders(std::vector<Pair<Shared<Shader>, ShaderType>>& _shaders)
	//{
	//	for (auto [shader, type] : _shaders)
	//	{
	//		SetShader(shader, type);
	//	}
	//}
	//void PipelineState::SetShader(Shared<Shader> _shader, ShaderType _type)
	//{
	//	switch (_type)
	//	{
	//	case ShaderType::None:break;
	//	case ShaderType::Vertex:
	//	{
	//		if (_shader->GetType() != ShaderType::Vertex)return;
	//		vertexShader = _shader;
	//		break;
	//	}
	//	case ShaderType::Hull:
	//	{
	//		hullShader = _shader;
	//		break;
	//	}
	//	case ShaderType::Domain:
	//	{
	//		domainShader = _shader;
	//		break;
	//	}
	//	case ShaderType::Geometry:
	//	{
	//		geometryShader = _shader;
	//		break;
	//	}
	//	case ShaderType::Pixel:
	//	{
	//		pixelShader = _shader;
	//		break;
	//	}
	//	default:
	//		break;
	//	}
	//}
}
