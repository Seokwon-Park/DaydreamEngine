#include "SteinsPCH.h"
#include "PipelineState.h"

namespace Steins
{
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
