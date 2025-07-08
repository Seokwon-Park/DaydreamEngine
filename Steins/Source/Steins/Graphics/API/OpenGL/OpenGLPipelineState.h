#pragma once

#include "Steins/Graphics/Core/PipelineState.h"
#include "glad/glad.h"

namespace Steins
{
	class OpenGLPipelineState : public PipelineState
	{
	public:
		OpenGLPipelineState(PipelineStateDesc _desc);
		virtual ~OpenGLPipelineState();

		virtual void Bind() const;
		virtual Shared<Material> CreateMaterial() override { return nullptr; };
	private:
		UInt32 inputDataIndex = 0;

		GLuint vao;
		GLuint pipeline;
	};
}
