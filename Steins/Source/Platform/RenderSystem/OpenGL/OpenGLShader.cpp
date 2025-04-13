#include "SteinsPCH.h"
#include "OpenGLShader.h"
#include "Steins/Enum/RendererEnums.h"

namespace Steins
{
	GLenum ShaderTypeToGLShader(ShaderType _shaderType)
	{
		switch (_shaderType)
		{
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderType::Geometry:
			return GL_GEOMETRY_SHADER;
		case ShaderType::Pixel:
			return GL_FRAGMENT_SHADER;
		default:
		{
			STEINS_CORE_ERROR("Unknown ShaderType");
			return 0;
		}
		}
	}

	ShaderType StringToShaderType(std::string _typeString)
	{
		if (_typeString == "vertex") return ShaderType::Vertex;
		else if (_typeString == "geometry") return ShaderType::Geometry;
		else if (_typeString == "pixel" || _typeString == "fragment") return ShaderType::Pixel;
		else
		{
			STEINS_CORE_ERROR("Invalid Shader Type String!");
			return ShaderType::None;
		}
	}

	OpenGLShader::OpenGLShader(const FilePath& _filepath, const ShaderType& _type)
	{
		std::ifstream file(_filepath.ToString());
		std::string readline;
		std::stringstream stringStream;

		ShaderType currentType = ShaderType::None;

		while (std::getline(file, readline))
		{
			//#type이라는 문자열을 줄에서 발견한 경우
			if (readline.find("#type") != std::string::npos)
			{
				size_t pos = readline.find("#type") + 6;
				std::string typeStr = readline.substr(pos);

				currentType = StringToShaderType(typeStr);

				shaderSources[currentType] = "";
			}
			else
			{
				if (currentType != ShaderType::None)
				{
					shaderSources[currentType] += readline + "\n";
				}
			}
		}
		Compile();

		//std::string source = ReadFile(_filepath);
		//auto shaderSources = PreProcess(source);
		//Compile(shaderSources);

		//// Extract name from filepath
		//auto lastSlash = _filepath.find_last_of("/\\");
		//lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		//auto lastDot = filepath.rfind('.');
		//auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		//m_Name = filepath.substr(lastSlash, count);
	}
	OpenGLShader::OpenGLShader(const std::string& _src, const ShaderType& _type)
	{
		shaderSources[_type] = _src;

		Compile();
	}
	OpenGLShader::~OpenGLShader()
	{
	}
	void OpenGLShader::Bind() const
	{
		glUseProgram(rendererID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}
	void OpenGLShader::SetMat4(const std::string& _name, const Matrix4x4& _value)
	{
		UploadUniformMat4(_name, _value);
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const Matrix4x4& matrix)
	{
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix.matrix[0][0]);
	}
	void OpenGLShader::Compile()
	{
		rendererID = glCreateProgram();
		std::vector<GLuint> glShaderIDs;

		for (auto [type, shaderSource] : shaderSources)
		{
			GLuint shader = glCreateShader(ShaderTypeToGLShader(type));
			const GLchar* source = shaderSource.c_str();

			glShaderSource(shader, 1, &source, 0);
			glCompileShader(shader);
			
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				STEINS_CORE_ERROR("{0}", infoLog.data());
				STEINS_CORE_ASSERT(false, "Shader compilation failure!");
				return;
			}
			glAttachShader(rendererID, shader);
			glShaderIDs.push_back(shader);

		}

		GLuint program = rendererID;
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (GLuint id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			STEINS_CORE_ERROR("{0}", infoLog.data());
			STEINS_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (GLuint id : glShaderIDs)
		{
			glDetachShader(program, id);

		}
	}
}
