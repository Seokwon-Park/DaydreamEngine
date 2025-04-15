#include "SteinsPCH.h"
#include "OpenGLPipelineState.h"

namespace Steins
{
	OpenGLPipelineState::~OpenGLPipelineState()
	{
		GLuint program = glCreateProgram();
		//	glAttachShader(rendererID, shader);
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
	void OpenGLPipelineState::Bind() const
	{
	}
}
