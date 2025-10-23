#include "DaydreamPCH.h"
#include "ModelRendererComponent.h"

#include "TransformComponent.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Core/ResourceManager.h"
#include "Daydream/Graphics/Resources/Skybox.h"

namespace Daydream
{
	ModelRendererComponent::ModelRendererComponent()
	{
		worldMatrix = ConstantBuffer::Create(sizeof(TransformConstantBufferData));
		materialCB = ConstantBuffer::Create(sizeof(MaterialConstantBufferData));
	}

	ModelRendererComponent::~ModelRendererComponent()
	{
	}

	void ModelRendererComponent::Init()
	{
		GetOwner()->GetScene()->AddModelRenderer(this);
	}

	void ModelRendererComponent::SetModel(Shared<Model> _model)
	{
		model = _model.get();
	}

	void ModelRendererComponent::Render()
	{
		Transform transform = GetOwner()->GetComponent<TransformComponent>()->GetTransform();
		TransformConstantBufferData data;
		data.world = transform.GetWorldMatrix().GetTranspose();
		data.invTranspose = data.world;
		data.invTranspose.MatrixInverse();
		data.invTranspose.MatrixTranspose();
		worldMatrix->Update(&data, sizeof(data));
		
		materialCB->Update(&materialValue, sizeof(materialValue));
		//for (auto mesh : model->GetMeshes())
		//{
		//	mesh->Bind();
		//	material->SetTexture2D("Texture", mesh->GetDiffuseTexture());
		//	material->SetTexture2D("NormalTexture", mesh->GetNormalTexture());
		//	
		//	material->Bind();
		//	Renderer::Submit(mesh->GetIndexCount());
		//}
		auto meshes = model->GetMeshes();
		auto materials = model->GetMaterials();
		for (int i = 0; i< model->GetMeshes().size(); i++)
		{
			meshes[i]->Bind();
			materials[i]->SetConstantBuffer("World", worldMatrix);
			materials[i]->SetConstantBuffer("Camera", GetOwner()->GetScene()->GetCurrentCamera()->GetViewProjectionConstantBuffer());
			//materials[i]->SetConstantBuffer("Lights", GetOwner()->GetScene()->GetLightConstantBuffer());
			//materials[i]->SetConstantBuffer("Material", materialCB);
			//materials[i]->SetTexture2D("BRDFLUT", GetOwner()->GetScene()->GetSkybox()->GetBRDF());
			//materials[i]->SetTextureCube("IrradianceTexture", GetOwner()->GetScene()->GetSkybox()->GetIrradianceTexture());
			//materials[i]->SetTextureCube("Prefilter", GetOwner()->GetScene()->GetSkybox()->GetPrefilterTexture());
			materials[i]->Bind();
			Renderer::Submit(meshes[i]->GetIndexCount());
		}
	}
}
