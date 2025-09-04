#include "DaydreamPCH.h"
#include "ResourceManager.h"

#include "Daydream/Graphics/Managers/MeshManager.h"
#include "Daydream/Graphics/Managers/ModelManager.h"
#include "Daydream/Graphics/Managers/ShaderManager.h"
#include "Daydream/Graphics/Managers/TextureManager.h"
#include "Daydream/Graphics/Managers/RenderPassManager.h"
#include "Daydream/Graphics/Managers/PipelineStateManager.h"

namespace Daydream
{
	ResourceManager::ResourceManager()
	{
		meshManager = MakeUnique<MeshManager>();
		shaderManager = MakeUnique<ShaderManager>();
		textureManager = MakeUnique<TextureManager>();
		renderPassManager = MakeUnique<RenderPassManager>();
		pipelineStateManager = MakeUnique<PipelineStateManager>();
	}

	void ResourceManager::Init()
	{
		GetInstance().textureManager->PreloadTexturesFromDirectory("Asset", true);
		GetInstance().shaderManager->LoadShadersFromDirectory("Asset/Shader", true);
		GetInstance().shaderManager->CreateEssentialShaderGroups();
		GetInstance().renderPassManager->CreateEssentialRenderPasses();
		GetInstance().pipelineStateManager->CreateEssentialPipelineStates();
	}
}
