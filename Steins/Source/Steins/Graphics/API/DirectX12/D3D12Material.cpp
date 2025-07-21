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
			D3D12_GPU_DESCRIPTOR_HANDLE handle{};
			handle.ptr = reinterpret_cast<UINT64>(texture->GetNativeHandle());
			device->GetCommandList()->SetGraphicsRootDescriptorTable(bindingMap[name].set, handle);
		}

		for (auto [name, cbuffer] : cbuffers)
		{
			if (cbuffer== nullptr) continue;
			device->GetCommandList()->SetGraphicsRootConstantBufferView(bindingMap[name].set, Cast<ID3D12Resource>(cbuffer->GetNativeHandle())->GetGPUVirtualAddress());
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