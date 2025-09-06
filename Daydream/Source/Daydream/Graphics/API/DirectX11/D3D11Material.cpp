#include "DaydreamPCH.h"
#include "D3D11Material.h"
#include "D3D11Buffer.h"
#include "D3D11Texture.h"
#include "D3D11TextureCube.h"
#include "D3D11Sampler.h"

namespace Daydream
{
	D3D11Material::D3D11Material(D3D11RenderDevice* _device, D3D11PipelineState* _pso)
	{
		device = _device;
		for (auto shader : _pso->GetShaders())
		{
			auto resourceInfo = shader->GetShaderReflectionData();
			for (auto& info : resourceInfo)
			{
				bindingMap[info.name] = info;
			}
		}
		//		switch (info.shaderResourceType)
		//		{
		//		case ShaderResourceType::ConstantBuffer:
		//		{
		//			cbuffers[info.name] = nullptr;
		//			break;
		//		}
		//		case ShaderResourceType::Texture:
		//		{
		//			textures[info.name] = nullptr;
		//			break;
		//		}
		//		case ShaderResourceType::Sampler:
		//		{
		//			break;
		//		}
		//		default:
		//			break;
		//		}
		//	}
		//}
	}

	//TODO:
	void D3D11Material::Bind()
	{
		for (auto [name, texture] : textures)
		{
			if (texture == nullptr) continue;
			auto resourceInfo = bindingMap[name];
			DAYDREAM_CORE_ASSERT(device->GetAPI() == RendererAPIType::DirectX11, "Wrong API!");
			Shared<D3D11Texture2D> d3d11Tex = static_pointer_cast<D3D11Texture2D>(texture);
			switch (bindingMap[name].shaderType)
			{
			case ShaderType::None:
				DAYDREAM_CORE_ASSERT(false, "ERROR");
				break;
			case ShaderType::Vertex:
				device->GetContext()->VSSetShaderResources(resourceInfo.binding, 1, d3d11Tex->GetSRV().GetAddressOf());
				break;
			case ShaderType::Hull:
				break;
			case ShaderType::Domain:
				break;
			case ShaderType::Geometry:
				break;
			case ShaderType::Pixel:
				device->GetContext()->PSSetShaderResources(resourceInfo.binding, 1, d3d11Tex->GetSRV().GetAddressOf());
				break;
			case ShaderType::Compute:
				break;
			default:
				break;
			}
		}

		for (auto [name, texture] : textureCubes)
		{
			if (texture == nullptr) continue;
			auto resourceInfo = bindingMap[name];
			DAYDREAM_CORE_ASSERT(device->GetAPI() == RendererAPIType::DirectX11, "Wrong API!");
			Shared<D3D11TextureCube> d3d11Tex = static_pointer_cast<D3D11TextureCube>(texture);
			switch (bindingMap[name].shaderType)
			{
			case ShaderType::None:
				DAYDREAM_CORE_ASSERT(false, "ERROR");
				break;
			case ShaderType::Vertex:
				device->GetContext()->VSSetShaderResources(resourceInfo.binding, 1, d3d11Tex->GetSRV().GetAddressOf());
				break;
			case ShaderType::Hull:
				break;
			case ShaderType::Domain:
				break;
			case ShaderType::Geometry:
				break;
			case ShaderType::Pixel:
				device->GetContext()->PSSetShaderResources(resourceInfo.binding, 1, d3d11Tex->GetSRV().GetAddressOf());
				break;
			case ShaderType::Compute:
				break;
			default:
				break;
			}
		}

		for (auto [name, sampler] : samplers)
		{
			if (sampler == nullptr) continue;
			auto resourceInfo = bindingMap[name];
			DAYDREAM_CORE_ASSERT(device->GetAPI() == RendererAPIType::DirectX11, "Wrong API!");
			Shared<D3D11Sampler> d3d11Sampler = static_pointer_cast<D3D11Sampler>(sampler);
			switch (bindingMap[name].shaderType)
			{
			case ShaderType::None:
				DAYDREAM_CORE_ASSERT(false, "ERROR");
				break;
			case ShaderType::Vertex:
				device->GetContext()->VSSetSamplers(resourceInfo.binding, 1, d3d11Sampler->GetSampler().GetAddressOf());
				break;
			case ShaderType::Hull:
				break;
			case ShaderType::Domain:
				break;
			case ShaderType::Geometry:
				break;
			case ShaderType::Pixel:
				device->GetContext()->PSSetSamplers(resourceInfo.binding, 1, d3d11Sampler->GetSampler().GetAddressOf());
				break;
			case ShaderType::Compute:
				break;
			default:
				break;
			}
		}

		for (auto [name, cbuffer] : cbuffers)
		{
			if (cbuffer == nullptr) continue;
			auto resourceInfo = bindingMap[name];
			//ID3D11Buffer* buffer = Cast<ID3D11Buffer>(cbuffer->GetNativeHandle());
			DAYDREAM_CORE_ASSERT(device->GetAPI() == RendererAPIType::DirectX11, "Wrong API!");
			auto d3d11Buffer = static_pointer_cast<D3D11ConstantBuffer>(cbuffer);
			switch (bindingMap[name].shaderType)
			{
			case ShaderType::None:
				break;
			case ShaderType::Vertex:
				device->GetContext()->VSSetConstantBuffers(resourceInfo.binding, 1, d3d11Buffer->GetBuffer().GetAddressOf());
				break;
			case ShaderType::Hull:
				break;
			case ShaderType::Domain:
				break;
			case ShaderType::Geometry:
				break;
			case ShaderType::Pixel:
				device->GetContext()->PSSetConstantBuffers(resourceInfo.binding, 1, d3d11Buffer->GetBuffer().GetAddressOf());
				break;
			case ShaderType::Compute:
				break;
			default:
				break;
			}
		}
	}

	void D3D11Material::SetSampler(const String& _name, Shared<Sampler> _sampler)
	{
		if (bindingMap.find(_name) != bindingMap.end())//&& textures.find(_name) != textures.end())
		{
			auto resourceInfo = bindingMap[_name];
			samplers[_name] = _sampler;
		}
	}

	void D3D11Material::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
	{
		if (bindingMap.find(_name) != bindingMap.end() )//&& textures.find(_name) != textures.end())
		{
			auto resourceInfo = bindingMap[_name];
			textures[_name] = _texture;
		}
	}

	void D3D11Material::SetTextureCube(const String& _name, Shared<TextureCube> _textureCube)
	{
		if (bindingMap.find(_name) != bindingMap.end() )//&& textureCubes.find(_name) != textureCubes.end())
		{
			auto resourceInfo = bindingMap[_name];
			textureCubes[_name] = _textureCube;
		}
	}
	void D3D11Material::SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer)
	{
		if (bindingMap.find(_name) != bindingMap.end() )//&& cbuffers.find(_name) != cbuffers.end())
		{
			auto resourceInfo = bindingMap[_name];
			cbuffers[_name] = _buffer;
		}
	}
}
