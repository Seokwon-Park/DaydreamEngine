#pragma once

#include "Daydream/Graphics/Resources/Model.h"

namespace Daydream
{
	class ModelManager
	{
	public:
		ModelManager();
	private:
		HashMap<String, Shared<Model>> meshCache;
	};
}