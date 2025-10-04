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

		// �ؽ�ó ���� ��ε�
		String albedoMapPath;
		//String specularTexturePath;
		String normalMapPath;
		String metallicMapPath;
		String roughnessMapPath;
		String AOMapPath;
		// ... PBR �ؽ�ó ��ε� (metallic, roughness, ao ...)

		// �ؽ�ó�� �ƴ� ��� ����
		Vector3 diffuseColor = { 0.8f, 0.8f, 0.8f };
		float shininess = 32.0f;
		// ... PBR ��� ���� (metallicFactor, roughnessFactor ...)
	};

	class Material : public Resource
	{
	public:
		virtual ~Material() = default;
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