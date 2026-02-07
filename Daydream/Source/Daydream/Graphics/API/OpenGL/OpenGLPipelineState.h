#pragma once

#include "Daydream/Graphics/Resources/PipelineState.h"
#include "glad/glad.h"

namespace Daydream
{
	class OpenGLPipelineState : public PipelineState
	{
	public:
		OpenGLPipelineState(PipelineStateDesc _desc);
		virtual ~OpenGLPipelineState();

		virtual void Bind() const;

		void BindPipelineState();

		//virtual Shared<Material> CreateMaterial() override;
	private:
		GLuint inputLayoutID;
		GLuint pipeline;
	};
}
