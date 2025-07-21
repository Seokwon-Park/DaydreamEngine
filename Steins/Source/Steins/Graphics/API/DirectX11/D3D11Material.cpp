#include "SteinsPCH.h"
#include "D3D11Material.h"

namespace Steins
{
	D3D11Material::D3D11Material(D3D11RenderDevice* _device, D3D11PipelineState* _pso)
	{
		device = _device;
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
	//TODO:
	void D3D11Material::Bind()
	{
		for (auto [name, texture] : textures)
		{
			if (texture == nullptr) continue;
			auto resourceInfo = bindingMap[name];
			ID3D11ShaderResourceView* srv = Cast<ID3D11ShaderResourceView>(texture->GetNativeHandle());
			switch (bindingMap[name].shaderType)
			{
			case Steins::ShaderType::None:
				break;
			case Steins::ShaderType::Vertex:
				break;
			case Steins::ShaderType::Hull:
				break;
			case Steins::ShaderType::Domain:
				break;
			case Steins::ShaderType::Geometry:
				break;
			case Steins::ShaderType::Pixel:
				device->GetContext()->PSSetShaderResources(resourceInfo.binding, 1, &srv);
				break;
			case Steins::ShaderType::Compute:
				break;
			default:
				break;
			}
		}
		for (auto [name, cbuffer] : cbuffers)
		{
			if (cbuffer == nullptr) continue;
			auto resourceInfo = bindingMap[name];
			ID3D11Buffer* buffer = Cast<ID3D11Buffer>(cbuffer->GetNativeHandle());
			switch (bindingMap[name].shaderType)
			{
			case Steins::ShaderType::None:
				break;
			case Steins::ShaderType::Vertex:
				device->GetContext()->VSSetConstantBuffers(resourceInfo.binding, 1, &buffer);
				break;
			case Steins::ShaderType::Hull:
				break;
			case Steins::ShaderType::Domain:
				break;
			case Steins::ShaderType::Geometry:
				break;
			case Steins::ShaderType::Pixel:
				break;
			case Steins::ShaderType::Compute:
				break;
			default:
				break;
			}
		}
	}

	void D3D11Material::SetTexture2D(const std::string& _name, Shared<Texture2D> _texture)
	{
		if (bindingMap.find(_name) != bindingMap.end() && textures.find(_name) != textures.end())
		{
			auto resourceInfo = bindingMap[_name];
			textures[_name] = _texture;
		}
	}
	void D3D11Material::SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer)
	{
		if (bindingMap.find(_name) != bindingMap.end() && cbuffers.find(_name) != cbuffers.end())
		{
			auto resourceInfo = bindingMap[_name];
			cbuffers[_name] = _buffer;
		}
	}
}
