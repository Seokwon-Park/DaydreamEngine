#pragma once

#include "Daydream/Core/ResourceManagerBase.h"

namespace Daydream
{
	class Shader;
	class ShaderGroup;
	
	class ShaderManager : public ResourceManagerBase<Shader>
	{
	public:
		ShaderManager();

		void LoadShadersFromDirectory(Path _directory, bool _isRecursive = false);
	};
}