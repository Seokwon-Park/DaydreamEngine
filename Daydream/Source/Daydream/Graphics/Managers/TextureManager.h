#pragma once

#include "Daydream/Graphics/Resources/Texture.h"

namespace Daydream
{
	class TextureManager
	{
	public:
		TextureManager() {};

		Shared<Texture2D> Get(Path _path)
		{
			_path = _path.make_preferred();
			return textureCache[_path.string()];
		}
		
		void PreloadTexturesFromDirectory(Path _directory, bool _isRecursive = false);
	private:
		Path assetDirectory;
		SortedMap<String, Shared<Texture2D>> textureCache;
	};
}