#include "SteinsPCH.h"
#include "OpenGLPipelineState.h"
#include "Platform/RenderSystem/GraphicsUtil.h"

#include "glm/gtc/type_ptr.hpp"

namespace Steins
{
	OpenGLPipelineState::OpenGLPipelineState(PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
		glCreateProgramPipelines(1, &pipeline);
		glBindProgramPipeline(pipeline);

		for (auto shader : shaders)
		{
			GLenum type = GraphicsUtil::GetGLShaderStage(shader->GetType());
			GLuint shaderID = static_cast<GLuint>(reinterpret_cast<uintptr_t>(shader->GetNativeHandle()));
			glUseProgramStages(pipeline, type, shaderID);
		}

		glValidateProgramPipeline(pipeline);
		GLint success;
		char infoLog[512];
		glGetProgramPipelineiv(pipeline, GL_VALIDATE_STATUS, &success);
		if (!success) {
			glGetProgramPipelineInfoLog(pipeline, 512, NULL, infoLog);
			STEINS_CORE_ERROR("pipeline validation failed!\n {0}", infoLog);
		}

		//GLuint shaderID = static_cast<GLuint>(reinterpret_cast<uintptr_t>(shader->GetNativeHandle()));
		//	glShaderIDs.push_back(shader);
			//rendererID = glCreateProgram();
			//std::vector<GLuint> glShaderIDs;

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
			//		std::vector<GLchar> infoLog(maxLength);
			//		glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			//		// We don't need the shader anymore.
			//		glDeleteShader(shader);

			//		STEINS_CORE_ERROR("{0}", infoLog.data());
			//		STEINS_CORE_ASSERT(false, "Shader compilation failure!");
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
			//	std::vector<GLchar> infoLog(maxLength);
			//	glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			//	// We don't need the program anymore.
			//	glDeleteProgram(program);
			//	// Don't leak shaders either.
			//	for (GLuint id : glShaderIDs)
			//	{
			//		glDeleteShader(id);
			//	}

			//	STEINS_CORE_ERROR("{0}", infoLog.data());
			//	STEINS_CORE_ASSERT(false, "Shader link failure!");
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
		GLuint vsid = static_cast<GLuint>(reinterpret_cast<uintptr_t>(vertexShader->GetNativeHandle()));


		//GLuint blockIndex = glGetUniformBlockIndex(vsid, "Camera");
		//if (blockIndex == GL_INVALID_INDEX) {
		//	// "Matrices" 블록을 찾지 못했거나 사용되지 않음
		//	std::cerr << "Error: Uniform block 'Camera' not found or inactive." << std::endl;
		//}
		//glUniformBlockBinding(vsid, blockIndex, 0);

		GLuint blockIdx = glGetUniformBlockIndex(vsid, "Camera");
		glUniformBlockBinding(vsid, blockIdx, 0);

		glBindProgramPipeline(pipeline);

		//glUseProgram(vsid);
		//GLint location = glGetUniformLocation(vsid, "u_ViewProjection");
		//glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(test));

	}
}
