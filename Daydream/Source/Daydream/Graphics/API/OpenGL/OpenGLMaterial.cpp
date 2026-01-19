#include "DaydreamPCH.h"
#include "OpenGLMaterial.h"
#include "OpenGLTexture.h"
#include "OpenGLTextureCube.h"
#include "OpenGLSampler.h"
#include "OpenGLBuffer.h"

namespace Daydream
{
	OpenGLMaterial::OpenGLMaterial(PipelineState* _pso)
	{
		for (auto shader : _pso->GetShaders())
		{
			auto resourceInfo = shader->GetShaderReflectionData();
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
				case ShaderResourceType::Texture:
				{
					textures[info.name] = nullptr;
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
			Shared<OpenGLTexture2D> glTextexture = static_pointer_cast<OpenGLTexture2D>(texture);
			glBindTextureUnit(bindingMap[name].binding, glTextexture->GetTextureID());
			glBindSampler(bindingMap[name].binding, glTextexture->GetSamplerID());
		}

		for (auto [name, texture] : textureCubes)
		{
			if (texture == nullptr) continue;
			Shared<OpenGLTextureCube> glTextexture = static_pointer_cast<OpenGLTextureCube>(texture);
			glBindTextureUnit(bindingMap[name].binding, glTextexture->GetTextureID());
			glBindSampler(bindingMap[name].binding, glTextexture->GetSamplerID());
		}

		for (auto [name, cbuffer] : cbuffers)
		{
			if (cbuffer == nullptr) continue;
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingMap[name].binding, static_cast<UInt32>(reinterpret_cast<uintptr_t>(cbuffer->GetNativeHandle())));
		}
	}
}
