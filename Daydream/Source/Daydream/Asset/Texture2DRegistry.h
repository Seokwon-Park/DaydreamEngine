#pragma once


namespace Daydream
{
	class Texture2D;

	class Texture2DRegistry
	{
	public:
		Texture2DRegistry() {};
			
		void CreateEssentialTextures();
		void LoadTexturesFromDirectory(Path _directory, bool _isRecursive = false);
	private:
		Path assetDirectory;
	};
}