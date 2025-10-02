#include "SkyboxPanel.h"

namespace Daydream
{
	SkyboxPanel::SkyboxPanel()
	{
		///////// For HDR Capture ///////////////////////////////////////////////////////////////////
		//cubeFaceViewMatrices =
		//{
		//	Matrix4x4::LookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f)),
		//	Matrix4x4::LookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f)),
		//	Matrix4x4::LookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		//	Matrix4x4::LookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
		//	Matrix4x4::LookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, 1.0f,  0.0f)),
		//	Matrix4x4::LookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, 1.0f,  0.0f))
		//};
		//cubeFaceProjMatrix = Matrix4x4::Perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		//cubeFaceConstantBuffers.resize(6);
		//for (int i = 0; i < 6; i++)
		//{
		//	cubeFaceConstantBuffers[i] = ConstantBuffer::Create(sizeof(Matrix4x4));
		//	captureViewProjections.push_back(cubeFaceProjMatrix * cubeFaceViewMatrices[i]);
		//	captureViewProjections[i].MatrixTranspose();
		//	cubeFaceConstantBuffers[i]->Update(&captureViewProjections[i], sizeof(Matrix4x4));
		//}

		//equirectangularRenderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		//equirectangularPSO = ResourceManager::GetResource<PipelineState>("EquirectangularPSO");

		//equirectangularMaterials.resize(6);
		//equirectangularResultTextures.resize(6);
		//for (int i = 0; i < 6; i++)
		//{
		//	equirectangularMaterials[i] = Material::Create(equirectangularPSO);
		//	equirectangularMaterials[i]->SetConstantBuffer("Camera", cubeFaceConstantBuffers[i]);
		//	equirectangularMaterials[i]->SetTexture2D("Texture", equirectangularTexture);
		//}

		//irradianceRenderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		//irradiancePSO = ResourceManager::GetResource<PipelineState>("IrradiancePSO");
		//irradianceMaterials.resize(6);
		//irradianceResultTextures.resize(6);
		//for (int i = 0; i < 6; i++)
		//{
		//	irradianceMaterials[i] = Material::Create(irradiancePSO);
		//	irradianceMaterials[i]->SetConstantBuffer("Camera", cubeFaceConstantBuffers[i]);
		//}
		equirectangularDropTarget = ResourceManager::GetResource<Texture2D>("Resource\\NoTexture.png");

		//brdfPSO = ResourceManager::GetResource<PipelineState>("BRDFPSO");

		//float vertices[] = {
		//	-1.0f,-1.0f, 0.0f, 0.0f, 1.0f,
		//	-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		//	1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		//	1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		//};

		//UInt32 indices[6] = { 0,1,2,2,3,0 };
		//resizeVB = VertexBuffer::CreateStatic(sizeof(vertices), 20, vertices);
		//resizeIB = IndexBuffer::Create(indices, 6);
		//quadMesh = Mesh::Create(resizeVB, resizeIB);

		//auto meshData = MeshGenerator::CreateCube();
		//Array<Vector3> positions;
		//for (Vertex v : meshData.vertices)
		//{
		//	positions.push_back(v.position);
		//}
		//boxVB = VertexBuffer::CreateStatic(sizeof(Vector3) * positions.size(), 12, positions.data());
		//boxIB = IndexBuffer::Create(meshData.indices.data(), meshData.indices.size());
		//boxMesh = Mesh::Create(boxVB, boxIB);
	}

	void SkyboxPanel::OnImGuiRender()
	{
		//TODO : 동기화 문제 방지용 텍스쳐가 쌓이는거 방지(이렇게 하는게 아닌듯 하지만 일단 임시 조치)
		if (oldTextures.size() > 100)
			oldTextures.clear();

		ImGui::Begin("SkyboxSettings");
		ImGui::Checkbox("Render Skybox", &isUsingSkybox);
		if (ImGui::Button("CreateEquirectangular"))
		{
			isHDR = true;
		}

		if (isHDR)
		{
			ImGui::Image(equirectangularDropTarget->GetImGuiHandle(), ImVec2{ 400,200 });
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_TEXTURE"))
				{
					String texturePath = (const char*)payload->Data;
					auto texture = ResourceManager::GetResource<Texture2D>(texturePath);
					skybox->GenerateHDRCubemap(texture);
					isHDR = false;
				}
				ImGui::EndDragDropTarget();
			}
		}

		if (equirectangularTexture != nullptr)
		{
			ImGui::Image(equirectangularTexture->GetImGuiHandle(), ImVec2{ 400,200 });
		}

		for (int i = 0; i < 6; i++)
		{
			ImGui::Text(faceLabels[i]);
			if (skybox->GetSkyboxTexture() != nullptr)
			{
				ImGui::Image(skybox->GetSkyboxTexture()->GetImGuiHandle(i), ImVec2{ 100,100 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_TEXTURE"))
					{
						String texturePath = (const char*)payload->Data;
						auto texture = ResourceManager::GetResource<Texture2D>(texturePath);

						skybox->UpdateSkyboxFace(i, texture);
					}
					ImGui::EndDragDropTarget();
				}
			}
		}
		//UI::DrawMaterialController("SkyboxTextures", skyboxMaterial.get());

		//if (ImGui::Button("BakeIrradiance"))
		//{
		//	skybox->GenerateIrradianceCubemap();
		//}

		//if (ImGui::Button("GenerateBRDF"))
		//{
		//	skybox->GenerateBRDF();
		//}

		for (int i = 0; i < 6; i++)
		{
			if (skybox->GetIrradianceTexture() != nullptr)
			{
				ImGui::Image(skybox->GetIrradianceTexture()->GetImGuiHandle(i), ImVec2{100,100});
			}
		}

		if (skybox->GetBRDF() != nullptr)
		{
			ImGui::Image(skybox->GetBRDF()->GetImGuiHandle(), ImVec2{ 100,100 });
		}

		ImGui::End();
	}

	//void SkyboxPanel::GenerateHDRCubemap()
	//{
	//	for (int i = 0; i < 6; i++)
	//	{
	//		equirectangularMaterials[i]->SetTexture2D("Texture", equirectangularTexture);
	//	}

	//	FramebufferDesc fbDesc;
	//	fbDesc.width = textureCube->GetWidth();
	//	fbDesc.height = textureCube->GetHeight();

	//	captureFramebuffer = Framebuffer::Create(equirectangularRenderPass, fbDesc);

	//	equirectangularResultTextures.clear();
	//	equirectangularResultTextures.resize(6);
	//	Daydream::TextureDesc textureDesc{};
	//	textureDesc.width = textureCube->GetWidth();
	//	textureDesc.height = textureCube->GetHeight();
	//	textureDesc.bindFlags = RenderBindFlags::ShaderResource;
	//	textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;
	//	for (int i = 0; i < 6; i++)
	//	{
	//		equirectangularRenderPass->Begin(captureFramebuffer);

	//		equirectangularPSO->Bind();
	//		boxMesh->Bind();
	//		equirectangularMaterials[i]->Bind();
	//		Renderer::Submit(boxMesh->GetIndexCount());

	//		equirectangularRenderPass->End();

	//		equirectangularResultTextures[i] = Texture2D::CreateEmpty(textureDesc);
	//		Renderer::GetRenderDevice()->CopyTexture2D(captureFramebuffer->GetColorAttachmentTexture(0), equirectangularResultTextures[i]);
	//		textureCube->Update(i, equirectangularResultTextures[i]);
	//		oldTextures.push_back(equirectangularResultTextures[i]);
	//	}
	//}
	//void SkyboxPanel::GenerateIrradianceCubemap()
	//{
	//	for (int i = 0; i < 6; i++)
	//		irradianceMaterials[i]->SetTextureCube("TextureCubemap", textureCube);

	//	FramebufferDesc fbDesc;
	//	fbDesc.width = 32;
	//	fbDesc.height = 32;

	//	irraidanceFramebuffer = Framebuffer::Create(irradianceRenderPass, fbDesc);

	//	irradianceResultTextures.clear();
	//	irradianceResultTextures.resize(6);
	//	Daydream::TextureDesc textureDesc{};
	//	textureDesc.width = 32;
	//	textureDesc.height = 32;
	//	textureDesc.bindFlags = RenderBindFlags::ShaderResource;
	//	textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;
	//	for (int i = 0; i < 6; i++)
	//	{
	//		irradianceRenderPass->Begin(irraidanceFramebuffer);

	//		irradiancePSO->Bind();
	//		boxMesh->Bind();
	//		irradianceMaterials[i]->Bind();
	//		Renderer::Submit(boxMesh->GetIndexCount());

	//		irradianceRenderPass->End();

	//		irradianceResultTextures[i] = Texture2D::CreateEmpty(textureDesc);
	//		Renderer::GetRenderDevice()->CopyTexture2D(irraidanceFramebuffer->GetColorAttachmentTexture(0), irradianceResultTextures[i]);
	//		//textureCube->Update(i, irradianceResultTextures[i]);
	//		oldTextures.push_back(irradianceResultTextures[i]);
	//	}
	//}
	//void SkyboxPanel::GenerateBRDF()
	//{
	//	FramebufferDesc fbDesc;
	//	fbDesc.width = textureCube->GetWidth();
	//	fbDesc.height = textureCube->GetHeight();

	//	captureFramebuffer = Framebuffer::Create(equirectangularRenderPass, fbDesc);

	//	quadMesh = Mesh::Create(resizeVB, resizeIB);

	//	TextureDesc textureDesc{};
	//	textureDesc.width = textureCube->GetWidth();
	//	textureDesc.height = textureCube->GetHeight();
	//	textureDesc.bindFlags = RenderBindFlags::ShaderResource;
	//	textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;

	//	equirectangularRenderPass->Begin(captureFramebuffer);

	//	brdfPSO->Bind();
	//	quadMesh->Bind();
	//	Renderer::Submit(quadMesh->GetIndexCount());

	//	equirectangularRenderPass->End();

	//	BRDFResult = Texture2D::CreateEmpty(textureDesc);
	//	Renderer::GetRenderDevice()->CopyTexture2D(captureFramebuffer->GetColorAttachmentTexture(0), BRDFResult);
	//}
}