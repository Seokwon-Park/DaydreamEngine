#pragma once

#include "Daydream/Graphics/Resources/Texture.h"
#include "Daydream/Core/ResourceManagerBase.h"


namespace Daydream
{
	class Texture2DManager : public ResourceManagerBase<Texture2D>
	{
	public:
		Texture2DManager() {};
			
		void LoadTexturesFromDirectory(Path _directory, bool _isRecursive = false);
	private:
		Path assetDirectory;
	};
}