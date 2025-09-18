#include "DaydreamPCH.h"
#include "SamplerManager.h"

namespace Daydream
{
	SamplerManager::SamplerManager()
	{
	}

	void SamplerManager::CreateEssentialSamplers()
	{
        SamplerDesc samplerDesc;
        samplerDesc.minFilter = FilterMode::Linear;
        samplerDesc.magFilter = FilterMode::Linear;
        samplerDesc.mipFilter = FilterMode::Linear;
        samplerDesc.wrapU = WrapMode::Repeat;
        samplerDesc.wrapV = WrapMode::Repeat;
        samplerDesc.wrapW = WrapMode::Repeat;
        registry["LinearRepeat"] = Sampler::Create(samplerDesc);

        // 선형 샘플러 (클램프)
        
        samplerDesc.minFilter = FilterMode::Linear;
        samplerDesc.magFilter = FilterMode::Linear;
        samplerDesc.mipFilter = FilterMode::Linear;
        samplerDesc.wrapU = WrapMode::ClampToEdge;
        samplerDesc.wrapV = WrapMode::ClampToEdge;
        samplerDesc.wrapW = WrapMode::ClampToEdge;
        registry["LinearClampToEdge"] = Sampler::Create(samplerDesc);

        // 가장 가까운 샘플러 (반복)
        
        samplerDesc.minFilter = FilterMode::Nearest;
        samplerDesc.magFilter = FilterMode::Nearest;
        samplerDesc.mipFilter = FilterMode::Nearest;
        samplerDesc.wrapU = WrapMode::Repeat;
        samplerDesc.wrapV = WrapMode::Repeat;
        samplerDesc.wrapW = WrapMode::Repeat;
        registry["NearestRepeat"] = Sampler::Create(samplerDesc);

        // 가장 가까운 샘플러 (클램프)
        
        samplerDesc.minFilter = FilterMode::Nearest;
        samplerDesc.magFilter = FilterMode::Nearest;
        samplerDesc.mipFilter = FilterMode::Nearest;
        samplerDesc.wrapU = WrapMode::ClampToEdge;
        samplerDesc.wrapV = WrapMode::ClampToEdge;
        samplerDesc.wrapW = WrapMode::ClampToEdge;
        registry["NearestClampToEdge"] = Sampler::Create(samplerDesc);
	}
	
}