#pragma once

#include "Steins/Render/PipelineState.h"
#include "glad/glad.h"

namespace Steins
{
	class OpenGLPipelineState : public PipelineState
	{
	public:
		OpenGLPipelineState(PipelineStateDesc _desc);
		virtual ~OpenGLPipelineState();

		virtual void Bind() const;

		virtual void AddConstantBuffer(Shared<ConstantBuffer> _buffer);
	private:
		GLuint pipeline;
	};
}
