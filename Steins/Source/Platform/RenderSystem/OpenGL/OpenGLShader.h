#pragma once

#include "Steins/Render/Shader.h"
#include "glad/glad.h"

namespace Steins
{
	class OpenGLShader :public Shader
	{
	public:
		OpenGLShader(const FilePath& _filepath);
		OpenGLShader(const std::string& _vertexSrc, const std::string& _pixelSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) override;

		void UploadUniformMat4(const std::string& name, const Matrix4x4& _matrix);
	private:
		std::string ParseGLSLFile(const FilePath& _filepath);
		void Compile();
	private:
		std::unordered_map<ShaderType, std::string> shaderSources;
		UInt32 rendererID;
	};
}

