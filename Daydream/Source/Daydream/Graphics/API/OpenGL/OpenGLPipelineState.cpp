#include "DaydreamPCH.h"
#include "OpenGLPipelineState.h"
#include "Daydream/Graphics/Utility/GraphicsUtility.h"
#include "OpenGLMaterial.h"

#include "OpenGLUtility.h"

#include "glm/gtc/type_ptr.hpp"

namespace Daydream
{

	

	OpenGLPipelineState::OpenGLPipelineState(PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
		glCreateVertexArrays(1, &vao);
		//for (const BufferElement& element : layout)
		UInt32 offset = 0;
		for(const auto& info : shaderGroup->GetShader(ShaderType::Vertex)->GetShaderReflectionData())
		{
			if (info.shaderResourceType != ShaderResourceType::Input) continue;
			glEnableVertexArrayAttrib(vao, inputDataIndex);
			glVertexArrayAttribFormat(vao, inputDataIndex,
				info.count,
				GraphicsUtility::OpenGL::ConvertRenderFormatToGLDataType(info.format),
				GL_FALSE,
				offset);
			glVertexArrayAttribBinding(vao, inputDataIndex, 0);
			offset += (UInt32)info.size;
			inputDataIndex++;
		}

		//glVertexArrayBindingDivisor(vao, 0, 0);

		glCreateProgramPipelines(1, &pipeline);
		glBindProgramPipeline(pipeline);

		GLint textureIndex = 0;
		//static GLint uboIndex = 0;
		for (auto shader : GetShaders())
		{
			GLenum type = GraphicsUtility::OpenGL::ConvertToShaderStageBit(shader->GetType());
			GLuint shaderID = static_cast<GLuint>(reinterpret_cast<uintptr_t>(shader->GetNativeHandle()));
			glUseProgramStages(pipeline, type, shaderID);
			for (auto& info : shader->GetShaderReflectionData())
			{
				switch (info.shaderResourceType)
				{
				case ShaderResourceType::ConstantBuffer:
				{
					//info.set = uboIndex;
					//glUniformBlockBinding(shaderID, info.binding, uboIndex++);
					break;
				}
				case ShaderResourceType::Texture:
				{
					break;
				}
				}
			}
		}

		glValidateProgramPipeline(pipeline);
		GLint success;
		char infoLog[512];
		glGetProgramPipelineiv(pipeline, GL_VALIDATE_STATUS, &success);
		if (!success) {
			glGetProgramPipelineInfoLog(pipeline, 512, NULL, infoLog);
			DAYDREAM_CORE_ERROR("pipeline validation failed!\n {0}", infoLog);
		}

		//GLuint samplerID;
		//glGenSamplers(1, &samplerID);
		//glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glBindSampler(0, samplerID);

		//GLuint shaderID = static_cast<GLuint>(reinterpret_cast<uintptr_t>(shader->GetNativeHandle()));
		//	glShaderIDs.push_back(shader);
			//rendererID = glCreateProgram();
			//Array<GLuint> glShaderIDs;

			//for (auto [type, shaderSource] : shaderSources)
			//{
			//	GLuint shader = glCreateShader(ShaderTypeToGLShader(type));
			//	const GLchar* source = shaderSource.c_str();

			//	glShaderSource(shader, 1, &source, 0);
			//	glCompileShader(shader);
			//	
			//	GLint isCompiled = 0;
			//	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			//	if (isCompiled == GL_FALSE)
			//	{
			//		GLint maxLength = 0;
			//		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			//		// The maxLength includes the NULL character
			//		Array<GLchar> infoLog(maxLength);
			//		glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			//		// We don't need the shader anymore.
			//		glDeleteShader(shader);

			//		DAYDREAM_CORE_ERROR("{0}", infoLog.data());
			//		DAYDREAM_CORE_ASSERT(false, "Shader compilation failure!");
			//		return;
			//	}
			//	glAttachShader(rendererID, shader);
			//	glShaderIDs.push_back(shader);

			//}

			//GLuint program = rendererID;
			//glLinkProgram(program);

			//GLint isLinked = 0;
			//glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
			//if (isLinked == GL_FALSE)
			//{
			//	GLint maxLength = 0;
			//	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			//	// The maxLength includes the NULL character
			//	Array<GLchar> infoLog(maxLength);
			//	glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			//	// We don't need the program anymore.
			//	glDeleteProgram(program);
			//	// Don't leak shaders either.
			//	for (GLuint id : glShaderIDs)
			//	{
			//		glDeleteShader(id);
			//	}

			//	DAYDREAM_CORE_ERROR("{0}", infoLog.data());
			//	DAYDREAM_CORE_ASSERT(false, "Shader link failure!");
			//	return;
			//}

			//for (GLuint id : glShaderIDs)
			//{
			//	glDetachShader(program, id);

			//}
	}
	OpenGLPipelineState::~OpenGLPipelineState()
	{

	}
	void OpenGLPipelineState::Bind() const
	{
		const auto& rsDesc = desc.rasterizerState;
		// Fill Mode (Solid vs Wireframe)
		GLenum polygonMode = (rsDesc.fillMode == FillMode::Wireframe) ? GL_LINE : GL_FILL;
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

		// Cull Mode
		if (rsDesc.cullMode == CullMode::None)
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glEnable(GL_CULL_FACE);
			// Front: GL_FRONT, Back: GL_BACK
			GLenum face = (rsDesc.cullMode == CullMode::Front) ? GL_FRONT : GL_BACK;
			glCullFace(face);
		}

		// Winding Order (Front Counter Clockwise)
		// true: CCW (반시계), false: CW (시계)
		glFrontFace(rsDesc.frontCounterClockwise ? GL_CCW : GL_CW);

		//// Depth Clip Enable
		//if (rsDesc.depthClipEnable)
		//{
		//	glDisable(GL_DEPTH_CLAMP); // Clipping 활성화 (기본 동작)
		//}
		//else
		//{
		//	glEnable(GL_DEPTH_CLAMP);  // Clipping 비활성화 (Clamping)
		//}

		//// Scissor Enable
		//if (rsDesc.scissorEnable)
		//	glEnable(GL_SCISSOR_TEST);
		//else
		//	glDisable(GL_SCISSOR_TEST);

		//// Multisample Enable
		//if (rsDesc.multisampleEnable)
		//	glEnable(GL_MULTISAMPLE);
		//else
		//	glDisable(GL_MULTISAMPLE);

		//// Antialiased Line Enable
		//if (rsDesc.antialiasedLineEnable)
		//	glEnable(GL_LINE_SMOOTH);
		//else
		//	glDisable(GL_LINE_SMOOTH);

		//// Depth Bias (Polygon Offset)
		//if (rsDesc.depthBias != 0 || rsDesc.slopeScaledDepthBias != 0.0f)
		//{
		//	glEnable(GL_POLYGON_OFFSET_FILL); // Wireframe일 경우 GL_POLYGON_OFFSET_LINE 고려 필요
		//	glPolygonOffset(rsDesc.slopeScaledDepthBias, static_cast<float>(rsDesc.depthBias));
		//}
		//else
		//{
		//	glDisable(GL_POLYGON_OFFSET_FILL);
		//}
		glBindVertexArray(vao);
		glBindProgramPipeline(pipeline);
	}
	//Shared<Material> OpenGLPipelineState::CreateMaterial()
	//{
	//	return MakeShared<OpenGLMaterial>(this);
	//}
}
