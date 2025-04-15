#include "SteinsPCH.h"
#include "PipelineState.h"

namespace Steins
{
	void PipelineState::SetShaders(std::vector<Pair<Shared<Shader>, ShaderType>>& _shaders)
	{
		for (auto [shader, type] : _shaders)
		{
			SetShader(shader, type);
		}
	}
	void PipelineState::SetShader(Shared<Shader> _shader, ShaderType _type)
	{
		switch (_type)
		{
		case Steins::ShaderType::None:break;
		case Steins::ShaderType::Vertex:
		{
			vertexShader = _shader;
			break;
		}
		case Steins::ShaderType::Hull:
		{
			hullShader = _shader;
			break;
		}
		case Steins::ShaderType::Domain:
		{
			vertexShader = _shader;
			break;
		}
		case Steins::ShaderType::Geometry:
		{
			vertexShader = _shader;
			break;
		}
		case Steins::ShaderType::Pixel:
		{
			vertexShader = _shader;
			break;
		}
		case Steins::ShaderType::Compute:
		{
			vertexShader = _shader;
			break;
		}
		default:
			break;
		}
	}
}
