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

        // ���� ���÷� (Ŭ����)
        
        samplerDesc.minFilter = FilterMode::Linear;
        samplerDesc.magFilter = FilterMode::Linear;
        samplerDesc.mipFilter = FilterMode::Linear;
        samplerDesc.wrapU = WrapMode::ClampToEdge;
        samplerDesc.wrapV = WrapMode::ClampToEdge;
        samplerDesc.wrapW = WrapMode::ClampToEdge;
        registry["LinearClampToEdge"] = Sampler::Create(samplerDesc);

        // ���� ����� ���÷� (�ݺ�)
        
        samplerDesc.minFilter = FilterMode::Nearest;
        samplerDesc.magFilter = FilterMode::Nearest;
        samplerDesc.mipFilter = FilterMode::Nearest;
        samplerDesc.wrapU = WrapMode::Repeat;
        samplerDesc.wrapV = WrapMode::Repeat;
        samplerDesc.wrapW = WrapMode::Repeat;
        registry["NearestRepeat"] = Sampler::Create(samplerDesc);

        // ���� ����� ���÷� (Ŭ����)
        
        samplerDesc.minFilter = FilterMode::Nearest;
        samplerDesc.magFilter = FilterMode::Nearest;
        samplerDesc.mipFilter = FilterMode::Nearest;
        samplerDesc.wrapU = WrapMode::ClampToEdge;
        samplerDesc.wrapV = WrapMode::ClampToEdge;
        samplerDesc.wrapW = WrapMode::ClampToEdge;
        registry["NearestClampToEdge"] = Sampler::Create(samplerDesc);
	}
	
}