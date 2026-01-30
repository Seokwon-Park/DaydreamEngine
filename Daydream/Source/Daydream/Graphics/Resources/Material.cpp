#include "DaydreamPCH.h"
#include "Material.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Resources/PipelineState.h"
#include "Daydream/Graphics/Manager/ResourceManager.h"

namespace Daydream
{
	Material::Material(Shared<ShaderGroup> _shaderGroup)
	{
		materialMap = _shaderGroup->GetMaterialMap();
	}

	Material::~Material()
	{
		textures.clear();
		cbuffers.clear();
		textureCubes.clear();
	}

	void Material::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
	{
		if (materialMap.find(_name) != materialMap.end())
		{
			if (!_texture->HasSampler())
			{
				_texture->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));
			}
			textures[_name] = _texture;
			
		}
	}

	void Material::SetTextureCube(const String& _name, Shared<TextureCube> _texture)
	{
		if (materialMap.find(_name) != materialMap.end())
		{
			textureCubes[_name] = _texture;
		}
	}

	void Material::SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _constantBuffer)
	{
		if (materialMap.find(_name) != materialMap.end())
		{
			cbuffers[_name] = _constantBuffer;
		}
	}

	Shared<Material> Material::Create(Shared<PipelineState> _pipeline)
	{
		return MakeShared<Material>(_pipeline->GetShaderGroup());
	}
}
