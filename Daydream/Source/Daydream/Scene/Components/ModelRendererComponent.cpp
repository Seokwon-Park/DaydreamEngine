#include "DaydreamPCH.h"
#include "ModelRendererComponent.h"

#include "TransformComponent.h"

#include "Daydream/Scene/GameEntity/GameEntity.h"
#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Manager/ResourceManager.h"
#include "Daydream/Graphics/Resources/Skybox.h"

namespace Daydream
{
	ModelRendererComponent::ModelRendererComponent()
	{
		worldMatrix = ConstantBuffer::Create(sizeof(TransformConstantBufferData));
		materialCB = ConstantBuffer::Create(sizeof(MaterialConstantBufferData));
		entityHandle = ConstantBuffer::Create(16);

		maskMaterial = Material::Create(ResourceManager::GetResource<PipelineState>("MaskPSO"));
	}

	ModelRendererComponent::~ModelRendererComponent()
	{
	}

	void ModelRendererComponent::Init()
	{
		Renderer::UpdateConstantBuffer(entityHandle, GetOwner()->GetHandle().id);
	}

	void ModelRendererComponent::SetModel(Shared<Model> _model)
	{
		model = _model.get();
	}

	//void ModelRendererComponent::Render()
	//{
	//	Transform transform = GetOwner()->GetComponent<TransformComponent>()->GetTransform();
	//	TransformConstantBufferData data;
	//	data.world = transform.GetLocalMatrix().GetTranspose();
	//	data.invTranspose = data.world.GetInversed().GetTranspose();
	//	worldMatrix->Update(&data, sizeof(TransformConstantBufferData));
	//	
	//	materialCB->Update(&materialValue, sizeof(materialValue));
	//	//for (auto mesh : model->GetMeshes())
	//	//{
	//	//	mesh->Bind();
	//	//	material->SetTexture2D("Texture", mesh->GetDiffuseTexture());
	//	//	material->SetTexture2D("NormalTexture", mesh->GetNormalTexture());
	//	//	
	//	//	material->Bind();
	//	//	Renderer::Submit(mesh->GetIndexCount());
	//	//}
	//	//auto meshes = model->GetMeshes();
	//	//auto materials = model->GetMaterials();
	//	//for (int i = 0; i< model->GetMeshes().size(); i++)
	//	//{
	//	//	meshes[i]->Bind();
	//	//	materials[i]->SetConstantBuffer("World", worldMatrix);
	//	//	materials[i]->SetConstantBuffer("Camera", GetOwner()->GetScene()->GetCurrentCamera()->GetViewProjectionConstantBuffer());
	//	//	materials[i]->SetConstantBuffer("Entity", entityHandle);

	//	//	//materials[i]->SetConstantBuffer("Lights", GetOwner()->GetScene()->GetLightConstantBuffer());
	//	//	//materials[i]->SetConstantBuffer("Material", materialCB);
	//	//	//materials[i]->SetTexture2D("BRDFLUT", GetOwner()->GetScene()->GetSkybox()->GetBRDF());
	//	//	//materials[i]->SetTextureCube("IrradianceTexture", GetOwner()->GetScene()->GetSkybox()->GetIrradianceTexture());
	//	//	//materials[i]->SetTextureCube("Prefilter", GetOwner()->GetScene()->GetSkybox()->GetPrefilterTexture());
	//	//	materials[i]->Bind();
	//	//	Renderer::Submit(meshes[i]->GetIndexCount());
	//	//}
	//}

	//void ModelRendererComponent::RenderMeshOnly()
	//{
	//	// 1. Transform 업데이트 (기존 로직 유지)
	//	Transform transform = GetOwner()->GetComponent<TransformComponent>()->GetTransform();
	//	TransformConstantBufferData data;
	//	data.world = transform.GetLocalMatrix().GetTranspose();
	//	data.invTranspose = data.world;
	//	data.invTranspose.Invert();
	//	data.invTranspose.Transpose();
	//	worldMatrix->Update(&data, sizeof(data));

	//	// 2. Mesh 순회 (Material Bind는 스킵!)
	//	//auto meshes = model->GetMeshes();
	//	maskMaterial->SetConstantBuffer("World", worldMatrix);
	//	maskMaterial->SetConstantBuffer("Camera", GetOwner()->GetScene()->GetCurrentCamera()->GetViewProjectionConstantBuffer());
	//	//for (int i = 0; i < meshes.size(); i++)
	//	//{
	//	//	// 2-1. Mesh(VBO, IBO) 바인딩
	//	//	meshes[i]->Bind();
	//	//	maskMaterial->Bind();

	//	//	// 3. 그리기 요청
	//	//	Renderer::Submit(meshes[i]->GetIndexCount());
	//	//}
	//}
}
