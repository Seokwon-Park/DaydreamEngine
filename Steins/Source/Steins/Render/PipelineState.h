#pragma once

#include "Shader.h"

namespace Steins
{
	class PipelineState
	{
	public:
		virtual ~PipelineState() = 0;

		virtual void Bind() const = 0;

		void SetShaders(std::vector<Pair<Shared<Shader>, ShaderType>>& _shaders);
		void SetShader(Shared<Shader> _shader, ShaderType _type);

	private:
		Shared<Shader> vertexShader;
		Shared<Shader> hullShader;
		Shared<Shader> domainShader;
		Shared<Shader> geometryShader;
		Shared<Shader> pixelShader;

		//root signature??

		VertexArray* vertexArray;//input layout

		//rtv, dsv;
		//blend, rast, ds;

	};
}