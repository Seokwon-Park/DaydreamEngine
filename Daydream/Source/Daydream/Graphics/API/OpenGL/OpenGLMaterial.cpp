#include "DaydreamPCH.h"
#include "OpenGLMaterial.h"
#include "OpenGLTexture.h"
#include "OpenGLBuffer.h"

namespace Daydream
{
	OpenGLMaterial::OpenGLMaterial(OpenGLPipelineState* _pso)
	{
		for (auto shader : _pso->GetShaders())
		{
			auto resourceInfo = shader->GetReflectionInfo();
			for (auto& info : resourceInfo)
			{
				bindingMap[info.name] = info;
				switch (info.shaderResourceType)
				{
				case ShaderResourceType::ConstantBuffer:
				{
					cbuffers[info.name] = nullptr;
					break;
				}
				case ShaderResourceType::Texture2D:
				{
					textures[info.name] = nullptr;
					break;
				}
				case ShaderResourceType::Sampler:
				{
					break;
				}
				default:
					break;
				}
			}
		}
	}
	void OpenGLMaterial::Bind()
	{
		for (auto [name, texture] : textures)
		{
			if (texture == nullptr) continue;
			glBindTextureUnit(bindingMap[name].binding, static_cast<UInt32>(reinterpret_cast<uintptr_t>(texture->GetNativeHandle())));

		}

		for (auto [name, cbuffer] : cbuffers)
		{
			if (cbuffer == nullptr) continue;
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingMap[name].set, static_cast<UInt32>(reinterpret_cast<uintptr_t>(cbuffer->GetNativeHandle())));

		}
	}
	void OpenGLMaterial::SetTexture2D(const std::string& _name, Shared<Texture2D> _texture)
	{
		if (bindingMap.find(_name) != bindingMap.end() && textures.find(_name) != textures.end())
		{
			textures[_name] = _texture;
		}
	}
	void OpenGLMaterial::SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer)
	{
		if (bindingMap.find(_name) != bindingMap.end() && cbuffers.find(_name) != cbuffers.end())
		{
			cbuffers[_name] = _buffer;
		}
	}
}
