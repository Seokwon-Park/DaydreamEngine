#include "SkyboxPanel.h"

namespace Daydream
{
	SkyboxPanel::SkyboxPanel()
	{
		equirectangularDropTarget = ResourceManager::GetResource<Texture2D>("Resource\\NoTexture.png");
	}

	void SkyboxPanel::OnImGuiRender()
	{
		ImGui::Begin("SkyboxSettings");
		ImGui::Checkbox("Render Skybox", &isUsingSkybox);

		if (ImGui::Button("CreateEquirectangular"))
		{
			isHDR = true;
		}

		ImGui::Text("BRDF");
		if (skybox->GetBRDF() != nullptr)
		{
			ImGui::Image(skybox->GetBRDF()->GetImGuiHandle(), ImVec2{ 100,100 });
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
				ImGui::SameLine();
				if (skybox->GetIrradianceTexture() != nullptr)
				{
					ImGui::Image(skybox->GetIrradianceTexture()->GetImGuiHandle(i), ImVec2{ 100,100 });
				}
				ImGui::SameLine();
				if (skybox->GetPrefilterTexture() != nullptr)
				{
					ImGui::Image(skybox->GetPrefilterTexture()->GetImGuiHandle(i), ImVec2{ 100,100 });
				}

			}
		}
		//UI::DrawMaterialController("SkyboxTextures", skyboxMaterial.get());

		if (ImGui::Button("UpdateIrradiance"))
		{
			skybox->GenerateIrradianceCubemap();
		}

		//if (ImGui::Button("GenerateBRDF"))
		//{
		//	skybox->GenerateBRDF();
		//}

		for (int i = 0; i < 6; i++)
		{

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