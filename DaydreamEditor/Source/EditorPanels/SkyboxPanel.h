#pragma once

#include "Daydream.h"
#include "UIPanel.h"

namespace Daydream
{
	class SkyboxPanel : public UIPanel
	{
	public:
		SkyboxPanel();

		void Setup(Shared<TextureCube> _texture, Shared<Material> _material)
		{
			textureCube = _texture;
			skyboxMaterial = _material;

			//GenerateBRDF();
		}
		
		void SetSkybox(Shared<Skybox> _skybox)
		{
			skybox = _skybox;
		}

		inline bool IsUsingSkybox() const  { return isUsingSkybox; };

		void OnImGuiRender();
	private:
		bool isUsingSkybox = true;
		bool isHDR = false;

		const char* faceLabels[6] = { "Right (+X)", "Left (-X)", "Top (+Y)", "Bottom (-Y)", "Front (+Z)", "Back (-Z)" };

		Shared<Skybox> skybox;

		Matrix4x4 cubeFaceProjMatrix;
		Array<Matrix4x4> cubeFaceViewMatrices;
		Array<Matrix4x4> captureViewProjections;
		Array<Shared<ConstantBuffer>> cubeFaceConstantBuffers;
		
		Array<Shared<Material>> equirectangularMaterials;
		Array<Shared<Material>> irradianceMaterials;
		Shared<Material> resizeMaterial;

		Shared<Framebuffer> captureFramebuffer;
		Shared<Framebuffer> resizeFramebuffer;
		Shared<Framebuffer> irraidanceFramebuffer;

		Shared<RenderPass> equirectangularRenderPass;
		Shared<RenderPass> resizeRenderPass;
		Shared<RenderPass> irradianceRenderPass;
		Shared<PipelineState> equirectangularPSO;
		Shared<PipelineState> irradiancePSO;
		Shared<PipelineState> resizePSO;
		Shared<PipelineState> brdfPSO;

		Shared<VertexBuffer> boxVB;
		Shared<IndexBuffer> boxIB;
		Shared<Mesh> boxMesh;
		Shared<VertexBuffer> resizeVB;
		Shared<IndexBuffer> resizeIB;
		Shared<Mesh> quadMesh;

		Shared<Texture2D> equirectangularTexture; 
		Shared<Texture2D> equirectangularDropTarget;
		Array<Shared<Texture2D>> equirectangularResultTextures;
		Array<Shared<Texture2D>> irradianceResultTextures;
		Shared<Texture2D> BRDFResult;

		Array<Shared<Texture2D>> oldTextures;

		Shared<TextureCube> textureCube;
		Shared<Material> skyboxMaterial;

		

		
	};
}