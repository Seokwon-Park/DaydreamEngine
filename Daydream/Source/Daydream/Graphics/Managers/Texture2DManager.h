#pragma once

#include "Daydream/Graphics/Resources/Texture.h"
#include "Daydream/Core/RegistryBase.h"


namespace Daydream
{
	class Texture2DManager : public RegistryBase<String, Shared<Texture2D>>
	{
	public:
		Texture2DManager() {};
			
		void CreateEssentialTextures();
		void LoadTexturesFromDirectory(Path _directory, bool _isRecursive = false);
	private:
		Path assetDirectory;
	};
}