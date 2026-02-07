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
		cubeFaceProjMatrix = Matrix4x4::CreatePerspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

		cubeFaceConstantBuffers.resize(6);
		for (int i = 0; i < 6; i++)
		{
			cubeFaceConstantBuffers[i] = ConstantBuffer::Create(sizeof(Matrix4x4));
			Matrix4x4 captureViewProjection = cubeFaceViewMatrices[i] * cubeFaceProjMatrix;
			captureViewProjection.Transpose();
				
			captureViewProjections.push_back(cubeFaceViewMatrices[i] * cubeFaceProjMatrix);
			captureViewProjections[i].Transpose();
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
		//boxVB = VertexBuffer::CreateStatic(sizeof(Vector3) * (UInt32)positions.size(), 12, positions.data());
		//boxIB = IndexBuffer::Create(meshData.indices.data(), (UInt32)meshData.indices.size());
		boxMesh = ResourceManager::GetResource<Mesh>("Box");
	}

	Skybox::~Skybox()
	{
		//oldTextures.clear();
		//skyboxTextureCube = nullptr;
		//equirectangularTexture = nullptr;
		//captureFramebuffer = nullptr;
		//equirectangularResultTextures.clear();

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
		textureDesc.type = TextureType::TextureCube;
		skyboxTextureCube = TextureCube::CreateEmpty(textureDesc);
		skyboxTextureCube->SetSampler(ResourceManager::GetResource<Sampler>("LinearClampToEdge"));
		//oldTextures.push_back(skyboxTextureCube);

		equirectangularTexture = _texture;

		//equirectangularMaterials[i]->SetTexture2D("Texture", equirectangularTexture);

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
		textureDesc.type = TextureType::Texture2D;
		for (int i = 0; i < 6; i++)
		{
			Renderer::BeginRenderPass(equirectangularRenderPass, captureFramebuffer);

			Renderer::BindPipelineState(equirectangularPSO);
			Renderer::SetConstantBuffer("Camera", cubeFaceConstantBuffers[i]);
			Renderer::SetTexture2D("Texture", equirectangularTexture);
			Renderer::BindMesh(boxMesh);
			Renderer::DrawIndexed(boxMesh->GetIndexCount());
			Renderer::EndRenderPass(equirectangularRenderPass);

			equirectangularResultTextures[i] = Texture2D::CreateEmpty(textureDesc);
			Renderer::CopyTexture2D(captureFramebuffer->GetColorAttachmentTexture(0), equirectangularResultTextures[i]);
			Renderer::CopyTextureToCubemapFace(skyboxTextureCube, i, equirectangularResultTextures[i], 0);
			skyboxTextureCube->Update(i, equirectangularResultTextures[i]);
			//oldTextures.push_back(equirectangularResultTextures[i]);
		}
		Renderer::GenerateMips(skyboxTextureCube);
	}

	void Skybox::GenerateIrradianceCubemap()
	{
		TextureDesc textureDesc{};
		textureDesc.width = diffuseResolution;
		textureDesc.height = diffuseResolution;
		textureDesc.mipLevels = 1;
		textureDesc.bindFlags = RenderBindFlags::ShaderResource;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;
		textureDesc.type = TextureType::TextureCube;

		irradianceTextureCube = TextureCube::CreateEmpty(textureDesc);
		irradianceTextureCube->SetSampler(ResourceManager::GetResource<Sampler>("LinearClampToEdge"));
		//oldTextures.push_back(irradianceTextureCube);

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
		textureDesc.type = TextureType::Texture2D;

		for (int i = 0; i < 6; i++)
		{
			Renderer::BeginRenderPass(irradianceRenderPass, irradianceFramebuffer);
			Renderer::BindPipelineState(irradiancePSO);
			Renderer::SetConstantBuffer("Camera", cubeFaceConstantBuffers[i]);
			Renderer::SetTextureCube("TextureCubemap", skyboxTextureCube);
			Renderer::BindMesh(boxMesh);
			Renderer::DrawIndexed(boxMesh->GetIndexCount());
			Renderer::EndRenderPass(irradianceRenderPass);

			irradianceResultTextures[i] = Texture2D::CreateEmpty(textureDesc);
			Renderer::CopyTexture2D(irradianceFramebuffer->GetColorAttachmentTexture(0), irradianceResultTextures[i]);
			Renderer::CopyTextureToCubemapFace(irradianceTextureCube, i, irradianceResultTextures[i], 0);
			irradianceTextureCube->Update(i, irradianceResultTextures[i]);
			//oldTextures.push_back(irradianceResultTextures[i]);
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
		textureDesc.type = TextureType::TextureCube;

		prefilterTextureCube = TextureCube::CreateEmpty(textureDesc);
		prefilterTextureCube->SetSampler(ResourceManager::GetResource<Sampler>("LinearClampToEdge"));
		//oldTextures.push_back(prefilterTextureCube);

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
			textureDesc.type = TextureType::Texture2D;

			FramebufferDesc fbDesc;
			fbDesc.width = std::max(1U, specularResolution >> mip);
			fbDesc.height = std::max(1U, specularResolution >> mip);

			prefilterFramebuffers[mip] = Framebuffer::Create(irradianceRenderPass, fbDesc);

			for (int face = 0; face < 6; face++)
			{
				UInt32 index = prefilterMipLevels * face + mip;

				Renderer::BeginRenderPass(irradianceRenderPass, prefilterFramebuffers[mip]);
				Renderer::BindPipelineState(prefilterPSO);
				Renderer::SetConstantBuffer("Camera", cubeFaceConstantBuffers[face]);
				Renderer::SetConstantBuffer("Roughness", roughnessConstantBuffers[mip]);
				Renderer::SetTextureCube("TextureCubemap", skyboxTextureCube);
				Renderer::BindMesh(boxMesh);
				Renderer::DrawIndexed(boxMesh->GetIndexCount());

				Renderer::EndRenderPass(irradianceRenderPass);

				prefilterResultTextures[index] = Texture2D::CreateEmpty(textureDesc);
				Renderer::CopyTexture2D(prefilterFramebuffers[mip]->GetColorAttachmentTexture(0), prefilterResultTextures[index]);
				Renderer::CopyTextureToCubemapFace(prefilterTextureCube, face, prefilterResultTextures[index], mip);
				if (mip == 0)
				{
					prefilterTextureCube->Update(face, prefilterResultTextures[index]);
				}
				//oldTextures.push_back(prefilterResultTextures[face]);
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
		textureDesc.type = TextureType::Texture2D;

		Renderer::BeginRenderPass(equirectangularRenderPass, captureFramebuffer);
		Renderer::BindPipelineState(brdfPSO);
		Renderer::BindMesh(quadMesh);
		Renderer::DrawIndexed(quadMesh->GetIndexCount());
		Renderer::EndRenderPass(equirectangularRenderPass);

		BRDFTexture = Texture2D::CreateEmpty(textureDesc);
		BRDFTexture->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));
		Renderer::CopyTexture2D(captureFramebuffer->GetColorAttachmentTexture(0), BRDFTexture);
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
		textureDesc.type = TextureType::Texture2D;

		resizeMaterial = Material::Create(resizePSO);

		Renderer::BeginRenderPass(resizeRenderPass, resizeFramebuffer);
		Renderer::BindPipelineState(resizePSO);
		Renderer::SetTexture2D("Texture", _texture);
		Renderer::BindMesh(quadMesh);
		Renderer::DrawIndexed(quadMesh->GetIndexCount());
		Renderer::EndRenderPass(resizeRenderPass);

		resizeResultTexture = Texture2D::CreateEmpty(textureDesc);
		Renderer::CopyTexture2D(resizeFramebuffer->GetColorAttachmentTexture(0), resizeResultTexture);
		Renderer::CopyTextureToCubemapFace(skyboxTextureCube, _faceIndex, resizeResultTexture, 0);
		skyboxTextureCube->Update(_faceIndex, resizeResultTexture);
		oldTextures.push_back(resizeResultTexture);

		skyboxTextureCube->GenerateMips();
	}
}