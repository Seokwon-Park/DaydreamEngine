#include "SteinsPCH.h"
#include "OpenGLShader.h"
#include "Steins/Enum/RendererEnums.h"

#include "Steins/Graphics/Utility/GraphicsUtil.h"

namespace Steins
{
	OpenGLShader::OpenGLShader(const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
	{
		shaderType = _type;
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
		GLuint shaderID = glCreateShader(GraphicsUtil::GetGLShaderType(shaderType));
		const GLchar* source = _src.c_str();

		glShaderSource(shaderID, 1, &source, nullptr);
		glCompileShader(shaderID);

		GLint isCompiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			Array<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);
			glDeleteShader(shaderID);

			STEINS_CORE_ERROR("{0}", infoLog.data());
			STEINS_CORE_ASSERT(false, "Shader compilation failure!");
			return;
		}

		shaderProgramID = glCreateShaderProgramv(GraphicsUtil::GetGLShaderType(shaderType), 1, &source);

		GLint isLinked = 0;
		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			Array<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(shaderProgramID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(shaderProgramID);
			// Don't leak shaders either.
			glDeleteShader(shaderID);

			STEINS_CORE_ERROR("{0}", infoLog.data());
			STEINS_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		GLint numUniformBlocks;
		glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);

		for (int i = 0; i < numUniformBlocks; i++)
		{
			GLsizei length;
			GLchar blockName[256];

			glGetActiveUniformBlockName(shaderProgramID, i, 256, &length, blockName);

			// 블록 크기와 바인딩 정보
			GLint blockSize;
			GLint blockBinding;
			glGetActiveUniformBlockiv(shaderProgramID, i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
			glGetActiveUniformBlockiv(shaderProgramID, i, GL_UNIFORM_BLOCK_BINDING, &blockBinding);

			ShaderResourceDesc desc{};
			desc.name = blockName;
			desc.type = ShaderResourceType::ConstantBuffer;
			desc.set = 0;
			desc.binding = blockBinding;
			desc.count = 1;
			desc.size = blockSize;

			resourceInfo.push_back(desc);
		}

		GLint numUniforms;
		glGetProgramInterfaceiv(shaderProgramID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
		for (int i = 0; i < numUniforms; i++)
		{
			GLchar name[256];
			GLsizei length;

			glGetProgramResourceName(shaderProgramID, GL_UNIFORM, i, 256, &length, name);

			// 상세 정보 쿼리
			GLenum properties[] = { GL_TYPE, GL_LOCATION, GL_ARRAY_SIZE, GL_OFFSET, GL_BLOCK_INDEX };
			GLint values[5];

			glGetProgramResourceiv(shaderProgramID, GL_UNIFORM, i, 5, properties, 5, nullptr, values);

			GLenum type = values[0];
			GLint location = values[1];
			GLint arraySize = values[2];
			GLint blockIndex = values[4];

			// uniform block에 속한 uniform은 제외
			if (blockIndex != -1) {
				continue;
			}

			// 샘플러 타입만 처리
			if (type == GL_SAMPLER_2D) {
				ShaderResourceDesc desc{};
				desc.name = std::string(name);
				desc.type = ShaderResourceType::Texture2D;
				desc.set = 0;
				desc.binding = location;
				desc.count = arraySize;
				desc.size = 0;

				resourceInfo.push_back(desc);
			}
		}

		glDeleteShader(shaderID);
	}
}
