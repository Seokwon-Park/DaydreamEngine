#pragma once

#include "Daydream/Graphics/Resources/Texture.h"
#include "Daydream/Graphics/Resources/Model.h"
#include "Daydream/Graphics/Resources/Shader.h"

namespace Daydream
{

	class AssetImporter
	{
	public:
		static Shared<Texture2D> LoadTexture2D(const Path& _texturePath);
		static Shared<Model> LoadModel(const Path& _modelPath);
		static Shared<Shader> LoadShader(const Path& _shaderPath);
	protected:

	private:

	};
}
