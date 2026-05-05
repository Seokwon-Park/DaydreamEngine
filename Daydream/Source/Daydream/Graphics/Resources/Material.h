#pragma once

#include "Daydream/Graphics/Resources/Shader.h"

namespace Daydream
{
	class ShaderGroup;
	class Sampler;
	class GraphicsPipelineState;
	class Texture2D;

	struct TextureBinding
	{
		AssetHandle handle = AssetHandle();
		Shared<Texture2D> cache = nullptr;
	};

	struct MaterialData
	{
		String name;

		// 텍스처 파일 경로들
		String albedoMapPath = "";
		//String specularTexturePath;
		String normalMapPath = "";
		// ... PBR 텍스처 경로들 (metallic, roughness, ao ...)
		String metallicMapPath = "";
		String roughnessMapPath = "";
		String AOMapPath = "";

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
		Vector3 albedo = Vector3(0.0f, 0.0f, 0.0f); // baseColor
		Float32 metallic = 0.0f;
	};


	class Material : public Asset
	{
	public:
		ASSET_CLASS_TYPE(Material)
		Material() = default;
		Material(Shared<ShaderGroup> _shaderGroup);
		~Material();
		void Bind() {};
		void Unbind() {};

		void SetTextureBinding(const String& _name, AssetHandle _textureHandle);
		void LoadMaterialAsset(const String& _name);
		//void SetTextureCube(const String& _name, Shared<TextureCube> _textureCube);
		//void SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _constantBuffer);

		inline const HashMap<String, TextureBinding>& GetTextureBindings() const { return textures; }
		//inline const HashMap<String, Shared<TextureCube>>& GetAllTextureCube() { return textureCubes; }
		//inline const HashMap<String, Shared<ConstantBuffer>>& GetAllConstantBuffer() { return cbuffers; }

		static Shared<Material> Create(Shared<GraphicsPipelineState> _pipeline);
	protected:
		HashMap<String, ShaderResourceType> textureBindingMap;

		/*HashMap<String, Shared<ConstantBuffer>> cbuffers;
		HashMap<String, Shared<TextureCube>> textureCubes;*/
		HashMap<String, TextureBinding> textures;
	private:
	};
}