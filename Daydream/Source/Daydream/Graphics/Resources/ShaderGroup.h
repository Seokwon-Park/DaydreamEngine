#pragma once

#include "Daydream/Graphics/Resources/Shader.h"

namespace Daydream
{
	class ShaderGroup
	{
	public:
		const Array<Shared<Shader>>& GetShaders();
		const Shared<Shader> GetShader(ShaderType _type);

		static Shared<ShaderGroup> Create(const Path& _vertexShaderPath, const Path&
_pixelShaderPath);
		static Shared<ShaderGroup> CreateBuiltin(const Path& _vertexShaderName);
		static Shared<ShaderGroup> CreateBuiltin(const Path& _vertexShaderName, const Path&
			_pixelShaderName);


		static Shared<ShaderGroup> Create(Shared<Shader> _vertexShader);
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

		const Array<ShaderReflectionData>& GetInputData() const { return inputReflectionData; }
		const HashMap<String, ShaderReflectionData>& GetShaderBindingMap() const { return shaderBindingMap; }
		const HashMap<String, ShaderResourceType>& GetMaterialMap() const { return materialMap; }
		const ShaderReflectionData* GetShaderBindingInfo(const String& _name) const;

		UInt32 GetSetCount() const { return setCount; }
	private:
		ShaderGroup(Shared<Shader> _vertexShader, Shared<Shader> _hullShader, Shared<Shader> _domainShader, Shared<Shader> _geometryShader, Shared<Shader> _pixelShader);

		void CreateInputReflectionData();
		void CreateShaderBindingMap();
		void CreateMaterialMap();

		void ValidateInterface() { /* ... */ }
		void GenerateUnifiedReflection() { /* ... */ }

		UInt32 setCount = 0;

		Shared<Shader> vertexShader = nullptr;
		Shared<Shader> hullShader = nullptr;
		Shared<Shader> domainShader = nullptr;
		Shared<Shader> geometryShader = nullptr;
		Shared<Shader> pixelShader = nullptr;

		Array<Shared<Shader>> shaders;

		Array<ShaderReflectionData> inputReflectionData;
		HashMap<String, ShaderReflectionData> shaderBindingMap;
		HashMap<String, ShaderResourceType> materialMap;
	};
}
