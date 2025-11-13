#pragma once

#include "ResourceRegistry.h"

namespace Daydream
{
	class Mesh;

	class MeshRegistry : public ResourceRegistry<Mesh>
	{
	public:
		MeshRegistry();

		virtual void CreateBuiltinResources() override;
	private:


	};
}