#include "SteinsPCH.h"
#include "D3D12Material.h"
#include "D3D12Buffer.h"
#include "D3D12Texture.h"

namespace Steins
{
	D3D12Material::D3D12Material(D3D12RenderDevice* _device, D3D12PipelineState* _pso)
	{
		device = _device;
		pso = _pso;

		for (auto shader : _pso->GetShaders())
		{
			auto resourceInfo = shader->GetReflectionInfo();
			for (auto info : resourceInfo)
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
	void D3D12Material::Bind()
	{
		for (auto [name, texture] : textures)
		{
			if (texture == nullptr) continue;
			STEINS_CORE_ASSERT(device->GetAPI() == RendererAPIType::DirectX12, "Wrong API!");
			Shared<D3D12Texture2D> d3d12Tex = static_pointer_cast<D3D12Texture2D>(texture);
			device->GetCommandList()->SetGraphicsRootDescriptorTable(bindingMap[name].set, d3d12Tex->GetSRVGPUHandle());
		}

		for (auto [name, cbuffer] : cbuffers)
		{
			if (cbuffer== nullptr) continue;
			STEINS_CORE_ASSERT(device->GetAPI() == RendererAPIType::DirectX12, "Wrong API!");
			Shared<D3D12ConstantBuffer> d3d12Buffer= static_pointer_cast<D3D12ConstantBuffer>(cbuffer);
			device->GetCommandList()->SetGraphicsRootConstantBufferView(bindingMap[name].set, d3d12Buffer->GetGPUVirtualAddress());
		}
	}
	void D3D12Material::SetTexture2D(const std::string& _name, Shared<Texture2D> _texture)
	{
		if (bindingMap.find(_name) != bindingMap.end() && textures.find(_name) != textures.end())
		{
			textures[_name] = _texture;
		}

	}
	void D3D12Material::SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer)
	{
		
		if (bindingMap.find(_name) != bindingMap.end() && cbuffers.find(_name) != cbuffers.end())
		{
			cbuffers[_name] = _buffer;
		}
	}
}