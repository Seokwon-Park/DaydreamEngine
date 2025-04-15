#pragma once

#include "Steins/Render/PipelineState.h"
#include "glad/glad.h"

namespace Steins
{
	class OpenGLPipelineState : public PipelineState
	{
	public:
		virtual ~OpenGLPipelineState();

		virtual void Bind() const;
	private:
		GLuint shaderProgramID;
	};
}
