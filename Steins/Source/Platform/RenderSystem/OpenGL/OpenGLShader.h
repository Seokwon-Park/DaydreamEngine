#pragma once

#include "Steins/Render/Shader.h"
#include "glad/glad.h"

namespace Steins
{
	class OpenGLShader :public Shader
	{
	public:
		OpenGLShader(const FilePath& _filepath);
		OpenGLShader(const std::string& name, const std::string& Src);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		std::string ReadFile(const FilePath _filepath);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		UInt32 rendererID;
	};
}

