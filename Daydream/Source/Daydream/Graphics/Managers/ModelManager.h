#pragma once

#include "Daydream/Graphics/Resources/Model.h"

namespace Daydream
{
	class ModelManager
	{
	public:
		ModelManager();

		void LoadModelsFromDirectory(Path _directory, bool _isRecursive = false);
	private:
		HashMap<String, Shared<Model>> meshCache;
	};
}