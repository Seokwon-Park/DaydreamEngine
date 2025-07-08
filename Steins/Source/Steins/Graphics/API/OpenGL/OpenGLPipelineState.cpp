#include "SteinsPCH.h"
#include "OpenGLPipelineState.h"
#include "Steins/Graphics/Utility/GraphicsUtil.h"

#include "glm/gtc/type_ptr.hpp"

namespace Steins
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Steins::ShaderDataType::Float:    return GL_FLOAT;
		case Steins::ShaderDataType::Float2:   return GL_FLOAT;
		case Steins::ShaderDataType::Float3:   return GL_FLOAT;
		case Steins::ShaderDataType::Float4:   return GL_FLOAT;
		case Steins::ShaderDataType::Int:      return GL_INT;
		case Steins::ShaderDataType::Int2:     return GL_INT;
		case Steins::ShaderDataType::Int3:     return GL_INT;
		case Steins::ShaderDataType::Int4:     return GL_INT;
		case Steins::ShaderDataType::Mat3x3:     return GL_FLOAT;
		case Steins::ShaderDataType::Mat4x4:     return GL_FLOAT;
		case Steins::ShaderDataType::Bool:     return GL_BOOL;
		}

		STEINS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLPipelineState::OpenGLPipelineState(PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
		glCreateVertexArrays(1, &vao);

		glBindVertexArray(vao);
		const BufferLayout& layout = _desc.inputLayout;
		for (const BufferElement& element : layout)
		{
			glEnableVertexAttribArray(inputDataIndex);
			glVertexAttribPointer(inputDataIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(void*)element.offset);
			inputDataIndex++;
		}

		glCreateProgramPipelines(1, &pipeline);
		glBindProgramPipeline(pipeline);

		for (auto shader : shaders)
		{
			GLenum type = GraphicsUtil::GetGLShaderStage(shader->GetType());
			GLuint shaderID = static_cast<GLuint>(reinterpret_cast<uintptr_t>(shader->GetNativeHandle()));
			glUseProgramStages(pipeline, type, shaderID);
			GLint samplerLocation = glGetUniformLocation(shaderID, "u_Texture");

			if (samplerLocation != -1) {
				glProgramUniform1i(shaderID, samplerLocation, 0);
			}
		}

		glValidateProgramPipeline(pipeline);
		GLint success;
		char infoLog[512];
		glGetProgramPipelineiv(pipeline, GL_VALIDATE_STATUS, &success);
		if (!success) {
			glGetProgramPipelineInfoLog(pipeline, 512, NULL, infoLog);
			STEINS_CORE_ERROR("pipeline validation failed!\n {0}", infoLog);
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
			//	Array<GLchar> infoLog(maxLength);
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
		glBindVertexArray(vao);

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
