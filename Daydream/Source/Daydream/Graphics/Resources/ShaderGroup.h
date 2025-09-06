#pragma once

#include "Daydream/Graphics/Resources/Shader.h"
#include "Daydream/Core/Resource.h"

namespace Daydream
{
	class ShaderGroup : public Resource
	{
	public:
		const Array<Shared<Shader>>& GetShaders();
		const Shared<Shader> GetShader(ShaderType _type);

		static Shared<ShaderGroup> Create(const String& _vertexShaderName,
			const String& _pixelShaderName);

		static Shared<ShaderGroup> Create(Shared<Shader> _vertexShader,
			Shared<Shader> _pixelShader);
		static Shared<ShaderGroup> Create(Shared<Shader> _vertexShader,
			Shared<Shader> _geometryShader,
			Shared<Shader> _pixelShader);
		static Shared<ShaderGroup> Create(Shared<Shader> _vertexShader,
			Shared<Shader> _hullShader,
			Shared<Shader> _domainShader,
			Shared<Shader> _geometryShader,
			Shared<Shader> _pixelShader);

		const Array<ShaderReflectionData>& GetInputData() { return inputReflectionData; }
		const Array<ShaderReflectionData>& GetShaderResourceData() { return shaderResourceReflectionData; }
	private:
		ShaderGroup(Shared<Shader> _vertexShader, Shared<Shader> _hullShader, Shared<Shader> _domainShader, Shared<Shader> _geometryShader, Shared<Shader> _pixelShader);

		void CreateInputReflectionData();
		void CreateShaderResourceReflectionData();

		void ValidateInterface() { /* ... */ }
		void GenerateUnifiedReflection() { /* ... */ }

		Shared<Shader> vertexShader = nullptr;
		Shared<Shader> hullShader = nullptr;
		Shared<Shader> domainShader = nullptr;
		Shared<Shader> geometryShader = nullptr;
		Shared<Shader> pixelShader = nullptr;

		Array<Shared<Shader>> shaders;

		Array<ShaderReflectionData> inputReflectionData;
		Array<ShaderReflectionData> shaderResourceReflectionData;
	};
}
