#include "SteinsPCH.h"
#include "PipelineState.h"

#include "Steins/Core/Application.h"

#include "Renderer.h"

namespace Steins
{
	PipelineState::PipelineState(PipelineStateDesc _desc)
	{
		vertexShader = _desc.vertexShader;
		hullShader = _desc.hullShader;
		domainShader = _desc.domainShader;
		geometryShader = _desc.geometryShader;
		pixelShader = _desc.pixelShader;

		STEINS_CORE_ASSERT(vertexShader != nullptr && pixelShader != nullptr, "VS and PS are necessary!");
		shaders.push_back(vertexShader);
		shaders.push_back(pixelShader);
		if (hullShader != nullptr)shaders.push_back(hullShader);
		if (domainShader != nullptr)shaders.push_back(domainShader);
		if (geometryShader != nullptr)shaders.push_back(geometryShader);
	}
	Shared<PipelineState> PipelineState::Create(PipelineStateDesc _desc)
	{
		return Application::GetGraphicsDevice()->CreatePipelineState(_desc);
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
