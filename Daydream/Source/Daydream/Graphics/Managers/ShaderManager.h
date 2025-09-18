#pragma once

#include "Daydream/Core/RegistryBase.h"

namespace Daydream
{
	class Shader;
	class ShaderGroup;
	
	class ShaderManager : public RegistryBase<Shader>
	{
	public:
		ShaderManager();

		void LoadShadersFromDirectory(Path _directory, bool _isRecursive = false);
	};
}