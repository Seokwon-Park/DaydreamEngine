 #pragma once

#include "Daydream/Graphics/Resources/Model.h"

namespace Daydream
{
	class ModelRegistry 
	{
	public:
		ModelRegistry();

		void LoadModelsFromDirectory(Path _directory, bool _isRecursive = false);
	private:
	};
}