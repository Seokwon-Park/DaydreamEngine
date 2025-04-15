#pragma once

#include "Shader.h"

namespace Steins
{
	class PipelineState
	{
	public:
		virtual ~PipelineState() = 0;

		virtual void Bind() const = 0;
	private:
		Shader* vertexShader;
		Shader* hullShader;
		Shader* domainShader;
		Shader* geometryShader;
		Shader* pixelShader;

		//root signature??

		VertexArray* vertexArray;//input layout

		//rtv, dsv;
		//blend, rast, ds;

	};
}