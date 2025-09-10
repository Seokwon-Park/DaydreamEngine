#include "DaydreamPCH.h"
#include "ShaderGroup.h"

#include "Daydream/Core/ResourceManager.h"

namespace Daydream
{
	const Array<Shared<Shader>>& ShaderGroup::GetShaders()
	{
		return shaders;
	}
	Shared<ShaderGroup> ShaderGroup::Create(Shared<Shader> _vertexShader, Shared<Shader> _pixelShader)
	{
		return Create(_vertexShader, nullptr, _pixelShader);
	}

	Shared<ShaderGroup> ShaderGroup::Create(Shared<Shader> _vertexShader, Shared<Shader> _geometryShader, Shared<Shader> _pixelShader)
	{
		return Create(_vertexShader, nullptr, nullptr, _geometryShader, _pixelShader);
	}
	Shared<ShaderGroup> ShaderGroup::Create(Shared<Shader> _vertexShader, Shared<Shader> _hullShader, Shared<Shader> _domainShader, Shared<Shader> _geometryShader, Shared<Shader> _pixelShader)
	{
		Shared<ShaderGroup> shaderGroup = Shared<ShaderGroup>(new ShaderGroup(
			_vertexShader,
			_hullShader,
			_domainShader,
			_geometryShader,
			_pixelShader
		));

		shaderGroup->CreateInputReflectionData();
		shaderGroup->CreateShaderResourceReflectionData();

		return shaderGroup;
	}
	ShaderGroup::ShaderGroup(Shared<Shader> _vertexShader, Shared<Shader> _hullShader, Shared<Shader> _domainShader, Shared<Shader> _geometryShader, Shared<Shader> _pixelShader)
	{
		DAYDREAM_CORE_ASSERT(_vertexShader && _pixelShader, "VS ans PS can't be nullptr!");

		vertexShader = _vertexShader;
		hullShader = _hullShader;
		domainShader = _domainShader;
		geometryShader = _geometryShader;
		pixelShader = _pixelShader;

		shaders.push_back(vertexShader);
		shaders.push_back(pixelShader);
		if (hullShader != nullptr) shaders.push_back(hullShader);
		if (domainShader != nullptr) shaders.push_back(domainShader);
		if (geometryShader != nullptr) shaders.push_back(geometryShader);
	}

	void ShaderGroup::CreateInputReflectionData()
	{
		for (const ShaderReflectionData& data : vertexShader->GetShaderReflectionData())
		{
			if (data.shaderResourceType == ShaderResourceType::Input)
			{
				inputReflectionData.push_back(data);
			}
		}
	}

	void ShaderGroup::CreateShaderResourceReflectionData()
	{
		for (const Shared<Shader>& shader : shaders)
		{
			for (ShaderReflectionData data : shader->GetShaderReflectionData())
			{
				if (data.shaderResourceType != ShaderResourceType::Input)
				{
					shaderResourceReflectionData.push_back(data);
				}
			}
		}
	}

	const Shared<Shader> ShaderGroup::GetShader(ShaderType _type)
	{
		switch (_type)
		{
		case ShaderType::None:
			break;
		case ShaderType::Vertex:
			return vertexShader;
			break;
		case ShaderType::Hull:
			DAYDREAM_CORE_ASSERT(hullShader, "HS is nullptr!");
			return hullShader;
			break;
		case ShaderType::Domain:
			DAYDREAM_CORE_ASSERT(domainShader, "DS is nullptr!");
			return domainShader;
			break;
		case ShaderType::Geometry:
			DAYDREAM_CORE_ASSERT(geometryShader, "GS is nullptr!");
			return geometryShader;
			break;
		case ShaderType::Pixel:
			return pixelShader;
			break;
		case ShaderType::Compute:
			break;
		default:
			break;
		}
		DAYDREAM_CORE_ASSERT(false, "Unknown Shader Type!");
		return Shared<Shader>();
	}
	Shared<ShaderGroup> ShaderGroup::Create(const String& _vertexShaderName, const String& _pixelShaderName)
	{
		auto vertexShader = ResourceManager::GetResource<Shader>(_vertexShaderName);
		auto pixelShader = ResourceManager::GetResource<Shader>(_pixelShaderName);
		return Create(vertexShader, pixelShader);
	}
}