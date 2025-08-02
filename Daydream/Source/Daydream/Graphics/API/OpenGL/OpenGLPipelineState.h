#pragma once

#include "Daydream/Graphics/Core/PipelineState.h"
#include "glad/glad.h"

namespace Daydream
{
	class OpenGLPipelineState : public PipelineState
	{
	public:
		OpenGLPipelineState(PipelineStateDesc _desc);
		virtual ~OpenGLPipelineState();

		virtual void Bind() const;
		virtual Shared<Material> CreateMaterial() override;
	private:
		UInt32 inputDataIndex = 0;

		GLuint vao;
		GLuint pipeline;
	};
}
