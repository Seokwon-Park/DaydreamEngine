#include "SteinsPCH.h"
#include "OpenGLShader.h"
#include "Steins/Enum/RendererEnums.h"

#include "Platform/RenderSystem/GraphicsUtil.h"

namespace Steins
{
	OpenGLShader::OpenGLShader(const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
	{
		type = _type;
		std::string src = _src;
		if (_mode == ShaderLoadMode::File)
		{
			src = "";
			std::ifstream file(_src);
			STEINS_CORE_ASSERT(file.is_open() == true, "Failed to open file!");
			std::string readline;
			std::stringstream stringStream;

			ShaderType currentType = _type;

			while (std::getline(file, readline))
			{
				src += readline + "\n";
			}
		}
		Compile(src);
	}
	OpenGLShader::~OpenGLShader()
	{
	}
	void OpenGLShader::Bind() const
	{
	}
	void OpenGLShader::Unbind() const
	{
	}
	/*void OpenGLShader::SetMat4(const std::string& _name, const Matrix4x4& _value)
	{
		UploadUniformMat4(_name, _value);
	}*/

	//void OpenGLShader::UploadUniformMat4(const std::string& name, const Matrix4x4& matrix)
	//{
	//	GLint location = glGetUniformLocation(rendererID, name.c_str());
	//	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix.matrix[0][0]);
	//}
	void OpenGLShader::Compile(const std::string& _src)
	{
		GLuint shaderID = glCreateShader(GraphicsUtil::openGLShaderTypeMap[type]);
		const GLchar* source = _src.c_str();

		glShaderSource(shaderID, 1, &source, nullptr);
		glCompileShader(shaderID);

		GLint isCompiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);
			glDeleteShader(shaderID);

			STEINS_CORE_ERROR("{0}", infoLog.data());
			STEINS_CORE_ASSERT(false, "Shader compilation failure!");
			return;
		}

		shaderProgramID = glCreateProgram();
		glProgramParameteri(shaderProgramID, GL_PROGRAM_SEPARABLE, GL_TRUE);

		glAttachShader(shaderProgramID, shaderID);
		glLinkProgram(shaderProgramID);


		GLint isLinked = 0;
		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(shaderProgramID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(shaderProgramID);
			// Don't leak shaders either.
			glDeleteShader(shaderID);

			STEINS_CORE_ERROR("{0}", infoLog.data());
			STEINS_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		glDetachShader(shaderProgramID, shaderID);
		glDeleteShader(shaderID);
	}
}
