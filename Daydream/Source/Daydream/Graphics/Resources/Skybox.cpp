#include "DaydreamPCH.h"
#include "Skybox.h"

#include "Daydream/Asset/AssetManager.h"
#include "Daydream/Graphics/Manager/ResourceManager.h"
#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Utility/MeshGenerator.h"
#include "Daydream/Graphics/Manager/SamplerRegistry.h"

namespace Daydream
{
	Skybox::Skybox()
	{
		cubeFaceViewMatrices =
		{
			Matrix4x4::CreateLookToLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f,  0.0f,  0.0f), Vector3(0.0f, 1.0f,  0.0f)),
			Matrix4x4::CreateLookToLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(-1.0f,  0.0f,  0.0f), Vector3(0.0f, 1.0f,  0.0f)),
			Matrix4x4::CreateLookToLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  1.0f,  0.0f), Vector3(0.0f,  0.0f, -1.0f)),
			Matrix4x4::CreateLookToLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f,  0.0f), Vector3(0.0f,  0.0f, 1.0f)),
			Matrix4x4::CreateLookToLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  0.0f,  1.0f), Vector3(0.0f, 1.0f,  0.0f)),
			Matrix4x4::CreateLookToLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  0.0f, -1.0f), Vector3(0.0f, 1.0f,  0.0f))
		};
		cubeFaceProjMatrix = Matrix4x4::CreatePerspectiveLH(Math::DegreeToRadian(90.0f), 1.0f, 0.1f, 10.0f);

		cubeFaceConstantBuffers.resize(6);
		for (int i = 0; i < 6; i++)
		{
			cubeFaceConstantBuffers[i] = ConstantBuffer::Create(sizeof(Matrix4x4));
			Matrix4x4 captureViewProjection = cubeFaceViewMatrices[i] * cubeFaceProjMatrix;
			captureViewProjection.Transpose();

			captureViewProjections.push_back(cubeFaceViewMatrices[i] * cubeFaceProjMatrix);
			captureViewProjections[i].Transpose();
		}
	}

	Skybox::~Skybox()
	{
		//oldTextures.clear();
		//skyboxTextureCube = nullptr;
		//equirectangularTexture = nullptr;
		//captureFramebuffer = nullptr;
		//equirectangularResultTextures.clear();

		/*equirectangularMaterials.clear();
		irradianceMaterials.clear();
		prefilterMaterials.clear();
		resizeMaterial = nullptr;*/
	}

	void Skybox::CreateResources()
	{
		//equirectangularRenderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		equirectangularPSO = ResourceManager::GetResource<PipelineState>("EquirectangularPSO");

		// Resize Capture setup
		//resizeRenderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		resizePSO = ResourceManager::GetResource<PipelineState>("ResizePSO");

		//irradianceRenderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		irradiancePSO = ResourceManager::GetResource<PipelineState>("IrradiancePSO");

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

		//////////////////////////////////////Create Default Skybox TextureCubes;
		skyboxMipLevels = (UInt32)std::log2f((Float32)skyboxResolution);

		Texture2DDesc textureDesc{};
		textureDesc.width = skyboxResolution;
		textureDesc.height = skyboxResolution;
		textureDesc.mipLevels = 1;
		textureDesc.textureUsage = TextureUsage::ShaderResource | TextureUsage::RenderTarget;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;
		skyboxTextureCube = TextureCube::Create(textureDesc);

		TextureViewDesc rtvDesc{};
		TextureViewDesc srvDesc{};
		skyboxFaceRTVs.resize(6);
		skyboxFaceSRVs.resize(6);
		for (UInt32 i = 0; i < 6; i++)
		{
			rtvDesc.type = TextureViewType::RenderTarget;
			rtvDesc.baseMip = 0;
			rtvDesc.mipCount = 1;
			rtvDesc.baseLayer = i;
			rtvDesc.layerCount = 1;
			skyboxFaceRTVs[i] = TextureView::Create(skyboxTextureCube, rtvDesc);

			srvDesc.type = TextureViewType::ShaderResource;
			srvDesc.baseMip = 0;
			srvDesc.mipCount = 1;
			srvDesc.baseLayer = i;
			srvDesc.layerCount = 1;
			skyboxFaceSRVs[i] = TextureView::Create(skyboxTextureCube, rtvDesc);
		}

		//////////////////////////////////////Create Irradiance TextureCube

		textureDesc.width = diffuseResolution;
		textureDesc.height = diffuseResolution;
		textureDesc.mipLevels = 1;
		textureDesc.textureUsage = TextureUsage::ShaderResource | TextureUsage::RenderTarget;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;
		irradianceTextureCube = TextureCube::Create(textureDesc);

		irradianceRTV.resize(6);
		for (UInt32 i = 0; i < 6; i++)
		{
			rtvDesc.type = TextureViewType::RenderTarget;
			rtvDesc.baseMip = 0;
			rtvDesc.mipCount = 1;
			rtvDesc.baseLayer = i;
			rtvDesc.layerCount = 1;

			irradianceRTV[i] = TextureView::Create(irradianceTextureCube, rtvDesc);
		}

		//////////////////////////////////////Create Prefilter TextureCube

		prefilterMipLevels = (UInt32)std::log2f((Float32)specularResolution);

		textureDesc.width = specularResolution;
		textureDesc.height = specularResolution;
		textureDesc.mipLevels = prefilterMipLevels;
		textureDesc.textureUsage = TextureUsage::ShaderResource | TextureUsage::RenderTarget;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;

		prefilterTextureCube = TextureCube::Create(textureDesc);

		prefilterRTV.resize(6 * prefilterMipLevels);
		for (UInt32 mip = 0; mip < prefilterMipLevels; mip++)
		{
			for (UInt32 face = 0; face < 6; face++)
			{
				rtvDesc.type = TextureViewType::RenderTarget;
				rtvDesc.baseMip = mip;
				rtvDesc.mipCount = 1;
				rtvDesc.baseLayer = face;
				rtvDesc.layerCount = 1;

				prefilterRTV[prefilterMipLevels * face + mip] = TextureView::Create(prefilterTextureCube, rtvDesc);
			}
		}

		roughnessConstantBuffers.resize(prefilterMipLevels);
		for (UInt32 mip = 0; mip < prefilterMipLevels; mip++)
		{
			roughnessConstantBuffers[mip] = ConstantBuffer::Create(sizeof(Vector4));
		}

		///////////////////////////////////////////////////////BRDF

		textureDesc.width = skyboxTextureCube->GetWidth();
		textureDesc.height = skyboxTextureCube->GetHeight();
		textureDesc.mipLevels = 1;
		textureDesc.textureUsage = TextureUsage::ShaderResource | TextureUsage::RenderTarget;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;

		BRDFTexture = Texture2D::Create(textureDesc);
		rtvDesc.type = TextureViewType::RenderTarget;
		rtvDesc.baseMip = 0;
		rtvDesc.mipCount = 1;        
		rtvDesc.baseLayer = 0;
		rtvDesc.layerCount = 1;      
		BRDFRTV = TextureView::Create(BRDFTexture, rtvDesc);

		//////////////////////////////////////Create Resize TextureCube

		textureDesc.width = skyboxTextureCube->GetWidth();
		textureDesc.height = skyboxTextureCube->GetHeight();
		textureDesc.mipLevels = 1;
		textureDesc.textureUsage = Daydream::TextureUsage::ShaderResource | TextureUsage::RenderTarget;
		textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;

		resizeTexture = Texture2D::Create(textureDesc);

		rtvDesc.type = TextureViewType::RenderTarget;
		rtvDesc.baseMip = 0;
		rtvDesc.mipCount = 1;
		rtvDesc.baseLayer = 0;
		rtvDesc.layerCount = 1;
		resizeRTV = TextureView::Create(resizeTexture, rtvDesc);
	
	}

	void Skybox::GenerateDefault()
	{
		GenerateHDRCubemap(AssetManager::GetAssetByPath<Texture2D>("Resource/skybox.hdr"));
		GenerateBRDF();
		GenerateIrradianceCubemap();
		GeneratePrefilterCubemap();
	}

	void Skybox::Update()
	{
		//if (oldTextures.size() > 200)
		//{
		//	DAYDREAM_CORE_INFO("Clear Old SkyboxTextures");
		//	oldTextures.clear();
		//}
	}

	void Skybox::GenerateHDRCubemap(Shared<Texture2D> _texture)
	{
		equirectangularTexture = _texture;

		for (int i = 0; i < 6; i++)
		{
			Renderer::UpdateConstantBuffer(cubeFaceConstantBuffers[i], captureViewProjections[i]);

			RenderingInfo renderingInfo{};
			renderingInfo.renderArea.x = 0;
			renderingInfo.renderArea.y = 0;
			renderingInfo.renderArea.width = skyboxResolution;
			renderingInfo.renderArea.height = skyboxResolution;

			AttachmentDesc attachDesc{};
			attachDesc.view = skyboxFaceRTVs[i];

			renderingInfo.colorAttachments.push_back(attachDesc);

			Renderer::BeginRendering(renderingInfo);
			Renderer::BindPipelineState(equirectangularPSO);
			Renderer::SetConstantBuffer("Camera", cubeFaceConstantBuffers[i]);
			Renderer::BindShaderResourceView("Texture", equirectangularTexture->GetDefaultSRV(), SamplerRegistry::LinearClampToEdge);
			Renderer::BindMesh(boxMesh);
			Renderer::DrawIndexed(boxMesh->GetIndexCount());
			Renderer::EndRendering(renderingInfo);

			//Renderer::CopyTextureToCubemapFace(skyboxTextureCube, i, equirectangularResultTextures[i], 0);
		}
		//Renderer::GenerateMips(skyboxTextureCube);
	}

	void Skybox::GenerateIrradianceCubemap()
	{
		//irradianceTextureCube->SetSampler(ResourceManager::GetResource<Sampler>("LinearClampToEdge"));

		//for (int i = 0; i < 6; i++)
		//{
		//	Renderer::BeginRenderPass(irradianceRenderPass, irradianceFramebuffer);
		//	Renderer::BindPipelineState(irradiancePSO);
		//	Renderer::SetConstantBuffer("Camera", cubeFaceConstantBuffers[i]);
		//	Renderer::SetTextureCube("TextureCubemap", skyboxTextureCube);
		//	Renderer::BindMesh(boxMesh);
		//	Renderer::DrawIndexed(boxMesh->GetIndexCount());
		//	Renderer::EndRenderPass(irradianceRenderPass);

		//	//Renderer::CopyTexture2D(irradianceFramebuffer->GetColorAttachmentTexture(0), irradianceResultTextures[i]);
		//	Renderer::CopyTextureToCubemapFace(irradianceTextureCube, i, irradianceResultTextures[i], 0);
		//	irradianceTextureCube->Update(i, irradianceResultTextures[i]);
		//}
	}

	void Skybox::GeneratePrefilterCubemap()
	{
		//prefilterTextureCube->SetSampler(ResourceManager::GetResource<Sampler>("LinearClampToEdge"));

		/*	prefilterMaterials.resize(6 * prefilterMipLevels);
			for (UInt32 mip = 0; mip < prefilterMipLevels; mip++)
			{
				for (UInt32 face = 0; face < 6; face++)
				{
					UInt32 index = prefilterMipLevels * face + mip;
					prefilterMaterials[index] = Material::Create(prefilterPSO);
					prefilterMaterials[index]->SetConstantBuffer("Camera", cubeFaceConstantBuffers[face]);
				}
			}*/

		//TextureDesc textureDesc{};
		//for (UInt32 mip = 0; mip < prefilterMipLevels; mip++)
		//{
		//	roughness = Vector4(0.0f, 0.0f, 0.0f, (float)mip / (prefilterMipLevels - 1));
		//	Renderer::UpdateConstantBuffer(roughnessConstantBuffers[mip], roughness);
		//	for (int face = 0; face < 6; face++)
		//	{
		//		UInt32 index = prefilterMipLevels * face + mip;

		//		Renderer::BeginRenderPass(irradianceRenderPass, prefilterFramebuffers[mip]);
		//		Renderer::BindPipelineState(prefilterPSO);
		//		Renderer::SetConstantBuffer("Camera", cubeFaceConstantBuffers[face]);
		//		Renderer::SetConstantBuffer("Roughness", roughnessConstantBuffers[mip]);
		//		Renderer::SetTextureCube("TextureCubemap", skyboxTextureCube);
		//		Renderer::BindMesh(boxMesh);
		//		Renderer::DrawIndexed(boxMesh->GetIndexCount());

		//		Renderer::EndRenderPass(irradianceRenderPass);

		//		//Renderer::CopyTexture2D(prefilterFramebuffers[mip]->GetColorAttachmentTexture(0), prefilterResultTextures[index]);
		//		Renderer::CopyTextureToCubemapFace(prefilterTextureCube, face, prefilterResultTextures[index], mip);
		//		if (mip == 0)
		//		{
		//			prefilterTextureCube->Update(face, prefilterResultTextures[index]);
		//		}
		//		//oldTextures.push_back(prefilterResultTextures[face]);
		//	}
		//}

		//prefilterTextureCube->GenerateMips();
	}

	void Skybox::GenerateBRDF()
	{
		//FramebufferDesc fbDesc;
		//fbDesc.width = skyboxTextureCube->GetWidth();
		//fbDesc.height = skyboxTextureCube->GetHeight();

		//Renderer::BeginRenderPass(equirectangularRenderPass, captureFramebuffer);
		//Renderer::BindPipelineState(brdfPSO);
		//Renderer::BindMesh(quadMesh);
		//Renderer::DrawIndexed(quadMesh->GetIndexCount());
		//Renderer::EndRenderPass(equirectangularRenderPass);

		//BRDFTexture->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));
		//Renderer::CopyTexture2D(captureFramebuffer->GetColorAttachmentTexture(0), BRDFTexture);
	}

	void Skybox::UpdateSkyboxFace(UInt32 _faceIndex, Shared<Texture2D> _texture)
	{
		//Renderer::BeginRenderPass(resizeRenderPass, resizeFramebuffer);
		//Renderer::BindPipelineState(resizePSO);
		//Renderer::SetTexture2D("Texture", _texture);
		//Renderer::BindMesh(quadMesh);
		//Renderer::DrawIndexed(quadMesh->GetIndexCount());
		//Renderer::EndRenderPass(resizeRenderPass);

		////Renderer::CopyTexture2D(resizeFramebuffer->GetColorAttachmentTexture(0), resizeResultTexture);
		//Renderer::CopyTextureToCubemapFace(skyboxTextureCube, _faceIndex, resizeResultTexture, 0);
		//skyboxTextureCube->Update(_faceIndex, resizeResultTexture);
		////oldTextures.push_back(resizeResultTexture);

		//Renderer::GenerateMips(skyboxTextureCube);
	}
}