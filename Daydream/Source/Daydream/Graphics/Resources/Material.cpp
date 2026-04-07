#include "DaydreamPCH.h"
#include "Material.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Resources/PipelineState.h"
#include "Daydream/Graphics/Manager/ResourceManager.h"

namespace Daydream
{
	Material::Material(Shared<ShaderGroup> _shaderGroup)
	{
		textureBindingMap = _shaderGroup->GetMaterialMap();
	}

	Material::~Material()
	{
		textures.clear();
		//cbuffers.clear();
		//textureCubes.clear();
	}

	void Material::SetTextureBinding(const String& _name, AssetHandle _textureHandle)
	{
		if (textureBindingMap.find(_name) == textureBindingMap.end())
		{
			//if (!_texture->HasSampler())
			//{
			//	_texture->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));
			//}
			DAYDREAM_CORE_ERROR("[Material] There is no Texture Name {}", _name);
			return;
		}
		textures[_name].handle = _textureHandle;
	}

	void Material::LoadMaterialAsset(const String& _name)
	{
		if (textures.find(_name) == textures.end())
		{
			DAYDREAM_CORE_ERROR("[Material] There is no Texture Name {}", _name);
			return;
		}
		textures[_name].cache = AssetManager::GetAsset<Texture2D>(textures[_name].handle);
	}

	//void Material::SetTextureCube(const String& _name, Shared<TextureCube> _texture)
	//{
	//	if (materialMap.find(_name) != materialMap.end())
	//	{
	//		textureCubes[_name] = _texture;
	//	}
	//}

	//void Material::SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _constantBuffer)
	//{
	//	if (materialMap.find(_name) != materialMap.end())
	//	{
	//		cbuffers[_name] = _constantBuffer;
	//	}
	//}

	Shared<Material> Material::Create(Shared<PipelineState> _pipeline)
	{
		return MakeShared<Material>(_pipeline->GetShaderGroup());
	}
}
