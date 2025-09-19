#pragma once

#include "Daydream/Graphics/Resources/Model.h"
#include "Daydream/Core/RegistryBase.h"

namespace Daydream
{
	class ModelManager : public RegistryBase<String, Shared<Model>>
	{
	public:
		ModelManager();

		void LoadModelsFromDirectory(Path _directory, bool _isRecursive = false);
	private:
	};
}