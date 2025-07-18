#include "SteinsPCH.h"
#include "OpenGLMaterial.h"
#include "OpenGLTexture.h"
#include "OpenGLBuffer.h"

namespace Steins
{
	OpenGLMaterial::OpenGLMaterial()
	{
	}
	void OpenGLMaterial::SetTexture2D(const std::string& _name, Shared<Texture2D> _texture)
	{
		glBindTextureUnit(0, static_cast<UInt32>(reinterpret_cast<uintptr_t>(_texture->GetNativeHandle())));
	}
	void OpenGLMaterial::SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, static_cast<UInt32>(reinterpret_cast<uintptr_t>(_buffer->GetNativeHandle())));
	}
}
