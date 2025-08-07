#include "DaydreamPCH.h"
#include "ModelRendererComponent.h"

#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	ModelRendererComponent::ModelRendererComponent()
	{
	}

	ModelRendererComponent::~ModelRendererComponent()
	{
	}

	void ModelRendererComponent::Init()
	{
		GetOwner()->GetScene()->GetModelRenderers().push_back(this);
	}

	void ModelRendererComponent::SetModel(Shared<Model> _model)
	{
		model = _model;
	}

	void ModelRendererComponent::Render()
	{
		material->Bind();
		for (auto mesh : model->GetMeshes())
		{
			mesh->Bind();

			Renderer::Submit(mesh->GetIndexCount());
		}
	}
}
