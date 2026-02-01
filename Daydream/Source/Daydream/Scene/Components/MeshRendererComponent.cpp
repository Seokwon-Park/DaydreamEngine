#include "DaydreamPCH.h"
#include "MeshRendererComponent.h"

#include "TransformComponent.h"
#include "LightComponent.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Asset/AssetManager.h"
#include "Daydream/Graphics/Manager/ResourceManager.h"
#include "Daydream/Graphics/Resources/PipelineState.h"
#include "Daydream/Graphics/Resources/Skybox.h"

namespace Daydream
{
	MeshRendererComponent::MeshRendererComponent()
	{
		

		maskMaterial = Material::Create(ResourceManager::GetResource<PipelineState>("MaskPSO"));
		lightMaterial = Material::Create(ResourceManager::GetResource<PipelineState>("DepthPSO"));
	}

	MeshRendererComponent::~MeshRendererComponent()
	{
	}
	void MeshRendererComponent::Init()
	{
	}

	void MeshRendererComponent::Render()
	{
		//TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();
		//TransformConstantBufferData data;
		//data.world = transform->GetWorldMatrix().GetTranspose();
		//data.invTranspose = data.world;
		//data.invTranspose.Invert();
		//data.invTranspose.Transpose();
		//worldMatrixConstantBuffer->Update(&data, sizeof(TransformConstantBufferData));

		Shared<Mesh> mesh;
		Shared<Material> material;
		if (meshHandle.IsValid())
			mesh = AssetManager::GetAsset<Mesh>(meshHandle);
		if (materialHandle.IsValid())
			material = AssetManager::GetAsset<Material>(materialHandle);

		if (mesh)
		{
			Renderer::BindMesh(mesh);
		}
		if (material)
		{

			material->SetConstantBuffer("Camera", GetOwner()->GetScene()->GetCurrentCamera()->GetViewProjectionConstantBuffer());
			material->SetConstantBuffer("Lights", GetOwner()->GetScene()->GetLightConstantBuffer());
			material->SetTexture2D("BRDFLUT", GetOwner()->GetScene()->GetSkybox()->GetBRDF());
			material->SetTextureCube("IrradianceTexture", GetOwner()->GetScene()->GetSkybox()->GetIrradianceTexture());
			material->SetTextureCube("Prefilter", GetOwner()->GetScene()->GetSkybox()->GetPrefilterTexture());
			Renderer::SetMaterial(material);
		}
		if (mesh && material)
		{
		}

		//	material->SetTexture2D("Texture", mesh->GetDiffuseTexture());
		//	material->SetTexture2D("NormalTexture", mesh->GetNormalTexture());
		//	
		//	material->Bind();
		//	Renderer::Submit(mesh->GetIndexCount());
		//}
		//auto meshes = model->GetMeshes();
		//auto materials = model->GetMaterials();
		//for (int i = 0; i< model->GetMeshes().size(); i++)
		//{
		//	meshes[i]->Bind();
		//	materials[i]->SetConstantBuffer("World", worldMatrix);
		//	materials[i]->SetConstantBuffer("Camera", GetOwner()->GetScene()->GetCurrentCamera()->GetViewProjectionConstantBuffer());
		//	materials[i]->SetConstantBuffer("Entity", entityHandle);

		//	//materials[i]->SetConstantBuffer("Lights", GetOwner()->GetScene()->GetLightConstantBuffer());
		//	//materials[i]->SetConstantBuffer("Material", materialCB);
		//	//materials[i]->SetTexture2D("BRDFLUT", GetOwner()->GetScene()->GetSkybox()->GetBRDF());
		//	//materials[i]->SetTextureCube("IrradianceTexture", GetOwner()->GetScene()->GetSkybox()->GetIrradianceTexture());
		//	//materials[i]->SetTextureCube("Prefilter", GetOwner()->GetScene()->GetSkybox()->GetPrefilterTexture());
		//	materials[i]->Bind();
		//	Renderer::Submit(meshes[i]->GetIndexCount());
		//}
	}

	void MeshRendererComponent::RenderMeshOnly()
	{
		// 1. Transform 업데이트 (기존 로직 유지)
		//TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();
		//TransformConstantBufferData data;
		//data.world = transform->GetWorldMatrix().GetTranspose();
		//data.invTranspose = data.world;
		//data.invTranspose.Invert();
		//data.invTranspose.Transpose();
		//worldMatrixConstantBuffer->Update(&data, sizeof(data));

		Shared<Mesh> mesh;
		// 2. Mesh 순회 (Material Bind는 스킵!)				
		if (meshHandle.IsValid())
			mesh = AssetManager::GetAsset<Mesh>(meshHandle);
		//maskMaterial->SetConstantBuffer("World", worldMatrixConstantBuffer);
		maskMaterial->SetConstantBuffer("Camera", GetOwner()->GetScene()->GetCurrentCamera()->GetViewProjectionConstantBuffer());
		//lightMaterial->SetConstantBuffer("World", worldMatrixConstantBuffer);
		//if (GetOwner()->GetScene()->GetLightComponent())
		//{
		//	lightMaterial->SetConstantBuffer("LightSpace", GetOwner()->GetScene()->GetLightComponent()->GetLight().lightViewProjectionBuffer);
		//}

		if (mesh)
		{
			mesh->Bind();
			maskMaterial->Bind();
			Renderer::DrawIndexed(mesh->GetIndexCount());
		}
		//for (int i = 0; i < meshes.size(); i++)
		//{
		//	// 2-1. Mesh(VBO, IBO) 바인딩
		//	meshes[i]->Bind();
		//	maskMaterial->Bind();

		//	// 3. 그리기 요청
		//	Renderer::Submit(meshes[i]->GetIndexCount());
		//}
	}
	void MeshRendererComponent::RenderDepth()
	{
		// 1. Transform 업데이트 (기존 로직 유지)
		//TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();
		//TransformConstantBufferData data;
		//data.world = transform->GetWorldMatrix().GetTranspose();
		//data.invTranspose = data.world;
		//data.invTranspose.Invert();
		//data.invTranspose.Transpose();
		//worldMatrixConstantBuffer->Update(&data, sizeof(data));

		Shared<Mesh> mesh;
		// 2. Mesh 순회 (Material Bind는 스킵!)				
		if (meshHandle.IsValid())
			mesh = AssetManager::GetAsset<Mesh>(meshHandle);

		//lightMaterial->SetConstantBuffer("World", worldMatrixConstantBuffer);



	}

}

