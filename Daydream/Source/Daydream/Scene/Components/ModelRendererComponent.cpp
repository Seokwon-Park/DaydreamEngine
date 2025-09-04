#include "DaydreamPCH.h"
#include "ModelRendererComponent.h"

#include "TransformComponent.h"

#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	ModelRendererComponent::ModelRendererComponent()
	{
		worldMatrix = ConstantBuffer::Create(sizeof(WorldConstantBuffer));
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
		WorldConstantBuffer data;
		data.world = transform.GetWorldMatrix();
		data.invTranspose = data.world;
		data.invTranspose.MatrixInverse();
		data.invTranspose.MatrixTranspose();
		worldMatrix->Update(&data, sizeof(data));
		material->SetConstantBuffer("World", worldMatrix);
		
		for (auto mesh : model->GetMeshes())
		{
			mesh->Bind();
			material->SetTexture2D("Texture", mesh->GetDiffuseTexture());
			material->SetTexture2D("NormalTexture", mesh->GetNormalTexture());
			material->Bind();
			Renderer::Submit(mesh->GetIndexCount());
		}
	}
}
