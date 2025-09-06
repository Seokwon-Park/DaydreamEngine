#pragma once

#include "Daydream/Graphics/Resources/Texture.h"
#include "Daydream/Core/ResourceManagerBase.h"


namespace Daydream
{
	class Texture2DManager : public ResourceManagerBase<Texture2D>
	{
	public:
		Texture2DManager() {};

		/*Shared<Texture2D> Get(Path _path)
		{
			_path = _path.make_preferred();
			return textureCache[_path.string()];
		}*/
		
		void LoadTexturesFromDirectory(Path _directory, bool _isRecursive = false);
	private:
		Path assetDirectory;
		//SortedMap<String, Shared<Texture2D>> textureCache;
	};
}