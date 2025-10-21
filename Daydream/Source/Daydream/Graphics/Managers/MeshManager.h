#pragma once

#include "Daydream/Graphics/Resources/Mesh.h"
#include "Daydream/Core/RegistryBase.h"

namespace Daydream
{
	class MeshManager : public RegistryBase<String, Shared<Mesh>>
	{
	public:
		MeshManager();

		void CreateEssentialMeshes();
	private:
	};
}