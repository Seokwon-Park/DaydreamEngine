#include "DaydreamPCH.h"
#include "AssetImporter.h"
#include "Daydream/Graphics/Utility/ImageLoader.h"
#include "Daydream/Graphics/Manager/ResourceManager.h"

namespace Daydream
{
	Shared<Texture2D> AssetImporter::LoadTexture2D(const Path& _texturePath)
	{
		Path texturePath = _texturePath;
		String pathString = texturePath.make_preferred().string();
		String extension = texturePath.extension().string();

		bool isSRGB = true;
		if (pathString.find("_n.") != std::string::npos ||
			pathString.find("_N.") != std::string::npos ||
			pathString.find("_normal.") != std::string::npos ||
			pathString.find("_Normal.") != std::string::npos ||
			pathString.find("metal") != std::string::npos ||    // metallic, metalness 등
			pathString.find("rough") != std::string::npos ||    // roughness
			pathString.find("ao.") != std::string::npos ||      // ambient occlusion
			pathString.find("height.") != std::string::npos)    // height map
		{
			isSRGB = false;
		}

		ImageLoader::ImageData data = ImageLoader::LoadImageFile(pathString);
		TextureDesc desc{};
		desc.bindFlags = RenderBindFlags::ShaderResource;
		desc.width = data.width;
		desc.height = data.height;
		if (extension == ".hdr")
		{
			desc.format = RenderFormat::R32G32B32A32_FLOAT;
		}
		else
		{
			// 8비트 텍스처 (sRGB 또는 Linear)
			desc.format = isSRGB ? RenderFormat::R8G8B8A8_UNORM_SRGB : RenderFormat::R8G8B8A8_UNORM;
		}
		auto newTexture = Texture2D::Create(data.GetRawDataPtr(), desc);
		newTexture->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));
		return newTexture;
	}
	Shared<Model> AssetImporter::LoadModel(const Path& _modelPath)
	{
		Path texturePath = _modelPath;
		String pathString = texturePath.make_preferred().string();
		String extension = texturePath.extension().string();
		Shared<Model> newModel = Model::Create();
		newModel->Load(pathString);

		return newModel;
	}
	Shared<Shader> AssetImporter::LoadShader(const Path& _shaderPath)
	{
		// 지원하는 확장자인지 확인
		Path entryPath = _shaderPath;
		String pathString = entryPath.string();
		String shaderName = entryPath.stem().string();
		String extension = entryPath.extension().string();
		ShaderType shaderType;
		if (pathString.find("VS.") != std::string::npos)
		{
			shaderType = ShaderType::Vertex;
		}
		if (pathString.find("PS.") != std::string::npos)
		{
			shaderType = ShaderType::Pixel;
		}

		if (pathString.find("HS.") != std::string::npos)
		{
			shaderType = ShaderType::Hull;
		}

		if (pathString.find("DS.") != std::string::npos)
		{
			shaderType = ShaderType::Domain;
		}
		if (pathString.find("GS.") != std::string::npos)
		{
			shaderType = ShaderType::Geometry;
		}
		Shared<Shader> newShader = Shader::Create(pathString, shaderType, ShaderLoadMode::File);

		return newShader;
	}
}

