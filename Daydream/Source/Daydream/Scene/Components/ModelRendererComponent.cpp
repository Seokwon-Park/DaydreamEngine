#include "DaydreamPCH.h"
#include "ModelRendererComponent.h"

#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	ModelRendererComponent::ModelRendererComponent()
	{
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
