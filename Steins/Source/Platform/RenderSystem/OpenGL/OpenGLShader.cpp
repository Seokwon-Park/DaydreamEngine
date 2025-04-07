#include "SteinsPCH.h"
#include "OpenGLShader.h"

namespace Steins
{
	OpenGLShader::OpenGLShader(const FilePath& _filepath)
	{
		std::string source = ReadFile(_filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = _filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}
}
