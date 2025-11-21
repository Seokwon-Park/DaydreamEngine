#include "DaydreamPCH.h"
#include "D3D12Material.h"
#include "D3D12Buffer.h"
#include "D3D12Texture.h"
#include "D3D12TextureCube.h"

namespace Daydream
{
	D3D12Material::D3D12Material(D3D12RenderDevice* _device, D3D12PipelineState* _pso)
	{
		device = _device;
		pso = _pso;

		for (ShaderReflectionData data : pso->GetShaderGroup()->GetShaderResourceData())
		{
			bindingMap[data.name] = data;
			//switch (info.shaderResourceType)
			//{
			//case ShaderResourceType::ConstantBuffer:
			//{
			//	cbuffers[info.name] = nullptr;
			//	break;
			//}
			//case ShaderResourceType::Texture:
			//{
			//	textures[info.name] = nullptr;
			//	break;
			//}
			//case ShaderResourceType::Sampler:
			//{
			//	break;
			//}
			//default:
			//	break;
			//}
		}
	}
	void D3D12Material::Bind()
	{
		for (auto [name, texture] : textures)
		{
			if (texture == nullptr) continue;
			DAYDREAM_CORE_ASSERT(device->GetAPI() == RendererAPIType::DirectX12, "Wrong API!");
			Shared<D3D12Texture2D> d3d12Tex = static_pointer_cast<D3D12Texture2D>(texture);
			device->GetCommandList()->SetGraphicsRootDescriptorTable(bindingMap[name].descriptorTableIndex, d3d12Tex->GetSRVGPUHandle());
			String samplerName = name + "Sampler";
			if (bindingMap.find(samplerName) != bindingMap.end())
			{
				device->GetCommandList()->SetGraphicsRootDescriptorTable(bindingMap[samplerName].descriptorTableIndex, d3d12Tex->GetSamplerHandle());
			}
		}

		for (auto [name, texture] : textureCubes)
		{
			if (texture == nullptr) continue;
			DAYDREAM_CORE_ASSERT(device->GetAPI() == RendererAPIType::DirectX12, "Wrong API!");
			Shared<D3D12TextureCube> d3d12Tex = static_pointer_cast<D3D12TextureCube>(texture);
			device->GetCommandList()->SetGraphicsRootDescriptorTable(bindingMap[name].descriptorTableIndex, d3d12Tex->GetSRVGPUHandle());
			String samplerName = name + "Sampler";
			device->GetCommandList()->SetGraphicsRootDescriptorTable(bindingMap[samplerName].descriptorTableIndex, d3d12Tex->GetSamplerHandle());
		}

		for (auto [name, cbuffer] : cbuffers)
		{
			if (cbuffer == nullptr) continue;
			DAYDREAM_CORE_ASSERT(device->GetAPI() == RendererAPIType::DirectX12, "Wrong API!");
			Shared<D3D12ConstantBuffer> d3d12Buffer = static_pointer_cast<D3D12ConstantBuffer>(cbuffer);
			device->GetCommandList()->SetGraphicsRootConstantBufferView(bindingMap[name].descriptorTableIndex, d3d12Buffer->GetGPUVirtualAddress());
		}
	}
	//void D3D12Material::SetTexture2D(const std::string& _name, Shared<Texture2D> _texture)
	//{
	//	if (bindingMap.find(_name) != bindingMap.end()/* && textures.find(_name) != textures.end()*/)
	//	{
	//		textures[_name] = _texture;
	//	}

	//}
	//void D3D12Material::SetTextureCube(const std::string& _name, Shared<TextureCube> _textureCube)
	//{
	//	if (bindingMap.find(_name) != bindingMap.end())//&& textureCubes.find(_name) != textureCubes.end())
	//	{
	//		auto resourceInfo = bindingMap[_name];
	//		textureCubes[_name] = _textureCube;
	//	}
	//}
	//void D3D12Material::SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer)
	//{
	//	if (bindingMap.find(_name) != bindingMap.end()/* && cbuffers.find(_name) != cbuffers.end()*/)
	//	{
	//		cbuffers[_name] = _buffer;
	//	}
	//}
}