#include "DaydreamPCH.h"
#include "ComponentRegistry.h"

#include "LightComponent.h"
#include "MeshRendererComponent.h"

namespace Daydream
{
	void ComponentRegistry::Init()
	{
		DAYDREAM_CORE_ASSERT(!instance, "ComponentRegistry must be initialized only 1 time")
		instance = new ComponentRegistry();

		REGISTER_COMPONENT(LightComponent);
		REGISTER_COMPONENT(ModelRendererComponent);
		REGISTER_COMPONENT(MeshRendererComponent);
	}

	void ComponentRegistry::Shutdown()
	{
		delete instance;
	}
}