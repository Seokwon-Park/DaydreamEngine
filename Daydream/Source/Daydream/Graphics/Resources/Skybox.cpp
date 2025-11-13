#include "DaydreamPCH.h"
#include "Skybox.h"

#include "Daydream/Graphics/Manager/ResourceManager.h"
#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Utility/MeshGenerator.h"

namespace Daydream
{
	Skybox::Skybox()
	{
		cubeFaceViewMatrices =
		{
			Matrix4x4::CreateLookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f)),
			Matrix4x4::CreateLookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f)),
			Matrix4x4::CreateLookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			Matrix4x4::CreateLookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
			Matrix4x4::CreateLookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, 1.0f,  0.0f)),
			Matrix4x4::CreateLookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, 1.0f,  0.0f))
		};
		cubeFaceProjMatrix = Matrix4x4::Perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

		cubeFaceConstantBuffers.resize(6);
		for (int i = 0; i < 6; i++)
		{
			cubeFaceConstantBuffers[i] = ConstantBuffer::Create(sizeof(Matrix4x4));
			captureViewProjections.push_back(cubeFaceProjMatrix * cubeFaceViewMatrices[i]);
			captureViewProjections[i].MatrixTranspose();
			cubeFaceConstantBuffers[i]->Update(&captureViewProjections[i], sizeof(Matrix4x4));
		}


		equirectangularRenderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		equirectangularPSO = ResourceManager::GetResource<PipelineState>("EquirectangularPSO");

		equirectangularMaterials.resize(6);
		equirectangularResultTextures.resize(6);
		for (int i = 0; i < equirectangularMaterials.size(); i++) 
		{
			equirectangularMaterials[i] = Material::Create(equirectangularPSO);
			equirectangularMaterials[i]->SetConstantBuffer("Camera", cubeFaceConstantBuffers[i]);
			equirectangularMaterials[i]->SetTexture2D("Texture", equirectangularTexture);
		}

		// Resize Capture setup
		resizeRenderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		resizePSO = ResourceManager::GetResource<PipelineState>("ResizePSO");

		irradianceRenderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		irradiancePSO = ResourceManager::GetResource<PipelineState>("IrradiancePSO");
		irradianceMaterials.resize(6);
		irradianceResultTextures.resize(6);
		for (int i = 0; i < 6; i++)
		{
			irradianceMaterials[i] = Material::Create(irradiancePSO);
			irradianceMaterials[i]->SetConstantBuffer("Camera", cubeFaceConstantBuffers[i]);
		}

		prefilterPSO = ResourceManager::GetResource<PipelineState>("PrefilterPSO");
		equirectangularDropTarget = AssetManager::GetAssetByPath<Texture2D>("Resource/NoTexture.png");

		brdfPSO = ResourceManager::GetResource<PipelineState>("BRDFPSO");

		quadMesh = ResourceManager::GetResource<Mesh>("Quad");

		auto meshData = MeshGenerator::CreateCube();
		Array<Vector3> positions;
		for (Vertex v : meshData.vertices)
		{
			positions.push_back(v.position);
		}
		boxVB = VertexBuffer::CreateStatic(sizeof(Vector3) * (UInt32)positions.size(), 12, positions.data());
		boxIB = IndexBuffer::Create(meshData.indices.data(), (UInt32)meshData.indices.size());
		boxMesh = Mesh::Create(boxVB, boxIB);
	}

	Skybox::~Skybox()
	{
		equirectangularMaterials.clear();
		irradianceMaterials.clear();
		prefilterMaterials.clear();
		resizeMaterial = nullptr;
	}

	void Skybox::Init()
	{
		//////////////////////////////////////Create Default Skybox TextureCubes;
		skyboxMipLevels = (UInt32)std::log2f((Float32)skyboxResolution);
		equirectangularTexture = AssetManager::GetAssetByPath<Texture2D>("Resource/skybox.hdr");
		GenerateHDRCubemap(equirectangularTexture);
		GenerateBRDF();
		GenerateIrradianceCubemap();
		GeneratePrefilterCubemap();
	}

	void Skybox::Update()
	{
		if (oldTextures.size() > 200)
		{
			DAYDREAM_CORE_INFO("Clear Old SkyboxTextures");
			oldTextures.clear();
		}
	}

	void Skybox::GenerateHDRCubemap(Shared<Texture2D> _texture)
	{
		skyboxMipLevels = (UInt32)std::log2f((Float32)skyboxResolution);

		TextureDesc textureDesc{};
		textureDesc.width = skyboxResolution;
		textureDesc.height = skyboxResolution;
		textureDesc.mipLevels = skyboxMipLevels;
		textureDesc.bindFlags = RenderBindFlags::ShaderResource;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;
		skyboxTextureCube = TextureCube::CreateEmpty(textureDesc);
		skyboxTextureCube->SetSampler(ResourceManager::GetResource<Sampler>("LinearClampToEdge"));
		oldTextures.push_back(skyboxTextureCube);

		equirectangularTexture = _texture;
		for (int i = 0; i < 6; i++)
		{
			equirectangularMaterials[i]->SetTexture2D("Texture", equirectangularTexture);
		}

		FramebufferDesc fbDesc;
		fbDesc.width = skyboxTextureCube->GetWidth();
		fbDesc.height = skyboxTextureCube->GetHeight();
		captureFramebuffer = Framebuffer::Create(equirectangularRenderPass, fbDesc);

		equirectangularResultTextures.clear();
		equirectangularResultTextures.resize(6);
		textureDesc.width = skyboxTextureCube->GetWidth();
		textureDesc.height = skyboxTextureCube->GetHeight();
		textureDesc.mipLevels = 1;
		textureDesc.bindFlags = RenderBindFlags::ShaderResource;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;
		for (int i = 0; i < 6; i++)
		{
			equirectangularRenderPass->Begin(captureFramebuffer);

			equirectangularPSO->Bind();
			boxMesh->Bind();
			equirectangularMaterials[i]->Bind();
			Renderer::Submit(boxMesh->GetIndexCount());

			equirectangularRenderPass->End();

			equirectangularResultTextures[i] = Texture2D::CreateEmpty(textureDesc);
			Renderer::GetRenderDevice()->CopyTexture2D(captureFramebuffer->GetColorAttachmentTexture(0), equirectangularResultTextures[i]);
			skyboxTextureCube->Update(i, equirectangularResultTextures[i]);
			oldTextures.push_back(equirectangularResultTextures[i]);
		}
		skyboxTextureCube->GenerateMips();
	}

	void Skybox::GenerateIrradianceCubemap()
	{
		TextureDesc textureDesc{};
		textureDesc.width = diffuseResolution;
		textureDesc.height = diffuseResolution;
		textureDesc.mipLevels = 1;
		textureDesc.bindFlags = RenderBindFlags::ShaderResource;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;

		irradianceTextureCube = TextureCube::CreateEmpty(textureDesc);
		irradianceTextureCube->SetSampler(ResourceManager::GetResource<Sampler>("LinearClampToEdge"));
		oldTextures.push_back(irradianceTextureCube);


		for (int i = 0; i < 6; i++)
		{
			irradianceMaterials[i]->SetTextureCube("TextureCubemap", skyboxTextureCube);
		}

		FramebufferDesc fbDesc;
		fbDesc.width = diffuseResolution;
		fbDesc.height = diffuseResolution;

		irradianceFramebuffer = Framebuffer::Create(irradianceRenderPass, fbDesc);

		irradianceResultTextures.clear();
		irradianceResultTextures.resize(6);
		textureDesc.width = diffuseResolution;
		textureDesc.height = diffuseResolution;
		textureDesc.bindFlags = RenderBindFlags::ShaderResource;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;

		for (int i = 0; i < 6; i++)
		{
			irradianceRenderPass->Begin(irradianceFramebuffer);

			irradiancePSO->Bind();
			boxMesh->Bind();
			irradianceMaterials[i]->Bind();
			Renderer::Submit(boxMesh->GetIndexCount());

			irradianceRenderPass->End();

			irradianceResultTextures[i] = Texture2D::CreateEmpty(textureDesc);
			Renderer::GetRenderDevice()->CopyTexture2D(irradianceFramebuffer->GetColorAttachmentTexture(0), irradianceResultTextures[i]);
			irradianceTextureCube->Update(i, irradianceResultTextures[i]);
			oldTextures.push_back(irradianceResultTextures[i]);
		}
	}

	void Skybox::GeneratePrefilterCubemap()
	{
		UInt32 result = (UInt32)std::log2f((Float32)specularResolution);

		prefilterMipLevels = result;

		prefilterMaterials.resize(6 * prefilterMipLevels);
		for (UInt32 mip = 0; mip < prefilterMipLevels; mip++)
		{
			for (UInt32 face = 0; face < 6; face++)
			{
				UInt32 index = prefilterMipLevels * face + mip;
				prefilterMaterials[index] = Material::Create(prefilterPSO);
				prefilterMaterials[index]->SetConstantBuffer("Camera", cubeFaceConstantBuffers[face]);
			}
		}

		roughnessConstantBuffers.resize(prefilterMipLevels);
		for (UInt32 mip = 0; mip < prefilterMipLevels; mip++)
		{
			roughnessConstantBuffers[mip] = ConstantBuffer::Create(sizeof(Vector4));
			roughness = Vector4(0.0f, 0.0f, 0.0f, (float)mip / (prefilterMipLevels - 1));
			roughnessConstantBuffers[mip]->Update(&roughness, sizeof(Vector4));
		}

		TextureDesc textureDesc{};
		textureDesc.width = specularResolution;
		textureDesc.height = specularResolution;
		textureDesc.mipLevels = prefilterMipLevels;
		textureDesc.bindFlags = RenderBindFlags::ShaderResource;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;

		prefilterTextureCube = TextureCube::CreateEmpty(textureDesc);
		prefilterTextureCube->SetSampler(ResourceManager::GetResource<Sampler>("LinearClampToEdge"));
		oldTextures.push_back(prefilterTextureCube);

		prefilterResultTextures.clear();
		prefilterResultTextures.resize(6 * prefilterMipLevels);
		prefilterFramebuffers.resize(prefilterMipLevels);
		for (UInt32 mip = 0; mip < prefilterMipLevels; mip++)
		{
			textureDesc.width = std::max(1U, specularResolution >> mip);
			textureDesc.height = std::max(1U, specularResolution >> mip);
			textureDesc.mipLevels = 1;
			textureDesc.bindFlags = RenderBindFlags::ShaderResource;
			textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;

			FramebufferDesc fbDesc;
			fbDesc.width = std::max(1U, specularResolution >> mip);
			fbDesc.height = std::max(1U, specularResolution >> mip);

			prefilterFramebuffers[mip] = Framebuffer::Create(irradianceRenderPass, fbDesc);

			for (int face = 0; face < 6; face++)
			{
				UInt32 index = prefilterMipLevels * face + mip;
				prefilterMaterials[index]->SetTextureCube("TextureCubemap", skyboxTextureCube);
				prefilterMaterials[index]->SetConstantBuffer("Roughness", roughnessConstantBuffers[mip]);

				irradianceRenderPass->Begin(prefilterFramebuffers[mip]);

				prefilterPSO->Bind();
				boxMesh->Bind();
				prefilterMaterials[index]->Bind();
				Renderer::Submit(boxMesh->GetIndexCount());

				irradianceRenderPass->End();

				prefilterResultTextures[index] = Texture2D::CreateEmpty(textureDesc);
				Renderer::GetRenderDevice()->CopyTexture2D(prefilterFramebuffers[mip]->GetColorAttachmentTexture(0), prefilterResultTextures[index]);
				if (mip == 0)
				{
					prefilterTextureCube->Update(face, prefilterResultTextures[index]);
				}
				else
				{
					Renderer::GetRenderDevice()->CopyTextureToCubemapFace(prefilterTextureCube.get(), face, prefilterResultTextures[index].get(), mip);
				}
				oldTextures.push_back(prefilterResultTextures[face]);
			}
		}


		//prefilterTextureCube->GenerateMips();
	}

	void Skybox::GenerateBRDF()
	{
		FramebufferDesc fbDesc;
		fbDesc.width = skyboxTextureCube->GetWidth();
		fbDesc.height = skyboxTextureCube->GetHeight();



		TextureDesc textureDesc{};
		textureDesc.width = skyboxTextureCube->GetWidth();
		textureDesc.height = skyboxTextureCube->GetHeight();
		textureDesc.bindFlags = RenderBindFlags::ShaderResource;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;

		equirectangularRenderPass->Begin(captureFramebuffer);

		brdfPSO->Bind();
		quadMesh->Bind();
		Renderer::Submit(quadMesh->GetIndexCount());

		equirectangularRenderPass->End();

		BRDFTexture = Texture2D::CreateEmpty(textureDesc);
		BRDFTexture->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));
		Renderer::GetRenderDevice()->CopyTexture2D(captureFramebuffer->GetColorAttachmentTexture(0), BRDFTexture);
	}

	void Skybox::UpdateSkyboxFace(UInt32 _faceIndex, Shared<Texture2D> _texture)
	{
		FramebufferDesc fbDesc;
		fbDesc.width = skyboxTextureCube->GetWidth();
		fbDesc.height = skyboxTextureCube->GetHeight();

		resizeFramebuffer = Framebuffer::Create(resizeRenderPass, fbDesc);
		//if (texture->GetWidth() != textureCube->GetWidth() ||
		//	texture->GetHeight() != textureCube->GetHeight())
		//{
		TextureDesc textureDesc{};
		textureDesc.width = skyboxTextureCube->GetWidth();
		textureDesc.height = skyboxTextureCube->GetHeight();
		textureDesc.bindFlags = Daydream::RenderBindFlags::ShaderResource;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;

		resizeMaterial = Material::Create(resizePSO);
		resizeMaterial->SetTexture2D("Texture", _texture);

		resizeRenderPass->Begin(resizeFramebuffer);
		resizePSO->Bind();
		quadMesh->Bind();
		resizeMaterial->Bind();
		Renderer::Submit(resizeIB->GetIndexCount());
		resizeRenderPass->End();

		resizeResultTexture = Texture2D::CreateEmpty(textureDesc);
		Renderer::GetRenderDevice()->CopyTexture2D(resizeFramebuffer->GetColorAttachmentTexture(0), resizeResultTexture);
		skyboxTextureCube->Update(_faceIndex, resizeResultTexture);
		oldTextures.push_back(resizeResultTexture);

		skyboxTextureCube->GenerateMips();
	}
}