#pragma once

#include "ResourceRegistry.h"

namespace Daydream
{
	class Shader;
		
	class ShaderManager
	{
	public:
		ShaderManager();

		void LoadShadersFromDirectory(Path _directory, bool _isRecursive = false);
	};
}