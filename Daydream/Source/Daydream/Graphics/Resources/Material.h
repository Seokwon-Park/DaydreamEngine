#pragma once

#include "Daydream/Core/Resource.h"
#include "Daydream/Graphics/Resources/Shader.h"


namespace Daydream
{
	class Sampler;
	class PipelineState;
	class Texture2D;
	class TextureCube;
	class ConstantBuffer;

	struct MaterialData
	{
		String name;

		// 텍스처 파일 경로들
		String albedoMapPath;
		//String specularTexturePath;
		String normalMapPath;
		String metallicMapPath;
		String roughnessMapPath;
		String AOMapPath;
		// ... PBR 텍스처 경로들 (metallic, roughness, ao ...)

		// 텍스처가 아닌 상수 값들
		Vector3 diffuseColor = { 0.8f, 0.8f, 0.8f };
		float shininess = 32.0f;
		// ... PBR 상수 값들 (metallicFactor, roughnessFactor ...)
	};

	struct MaterialConstantBufferData
	{
		UInt32 useAlbedoMap = 1;
		UInt32 useNormalMap = 1;
		UInt32 useAOMap = 1;
		UInt32 useMetallicMap = 1;
		UInt32 useRoughnessMap = 1;
		Float32 exposure = 1.0f;
		Float32 gamma = 2.2f;
		Float32 roughness = 0;
		Vector3 albedo = Vector3(0.0f,0.0f,0.0f); // baseColor
		Float32 metallic = 0.0f;
	};


	class Material : public Resource
	{
	public:
		virtual ~Material();
		virtual void Bind() = 0;
		
		void SetTexture2D(const String& _name, Shared<Texture2D> _texture);
		void SetTextureCube(const String& _name, Shared<TextureCube> _textureCube);
		void SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer);

		inline const HashMap<String, Shared<Texture2D>>& GetAllTexture2D() { return textures; }
		inline const HashMap<String, Shared<TextureCube>>& GetAllTextureCube() { return textureCubes; }

		static Shared<Material> Create(Shared<PipelineState> _pipeline);
	protected:
		HashMap<String, ShaderReflectionData> bindingMap;

		HashMap<String, Shared<ConstantBuffer>> cbuffers;
		HashMap<String, Shared<Texture2D>> textures;
		HashMap<String, Shared<TextureCube>> textureCubes;
	private:

	};
}