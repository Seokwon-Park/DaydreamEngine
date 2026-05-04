#include "DaydreamPCH.h"
#include "SamplerRegistry.h"

namespace Daydream
{
	SamplerRegistry::SamplerRegistry()
	{
	}

	void SamplerRegistry::CreateBuiltinResources()
	{
		SamplerDesc samplerDesc;
		samplerDesc.minFilter = FilterMode::Linear;
		samplerDesc.magFilter = FilterMode::Linear;
		samplerDesc.mipFilter = FilterMode::Linear;
		samplerDesc.wrapU = WrapMode::Repeat;
		samplerDesc.wrapV = WrapMode::Repeat;
		samplerDesc.wrapW = WrapMode::Repeat;
		LinearRepeat = Sampler::Create(samplerDesc);
		registry["LinearRepeat"] = LinearRepeat;


		// 선형 샘플러 (클램프)

		samplerDesc.minFilter = FilterMode::Linear;
		samplerDesc.magFilter = FilterMode::Linear;
		samplerDesc.mipFilter = FilterMode::Linear;
		samplerDesc.wrapU = WrapMode::ClampToEdge;
		samplerDesc.wrapV = WrapMode::ClampToEdge;
		samplerDesc.wrapW = WrapMode::ClampToEdge;
		LinearClampToEdge = Sampler::Create(samplerDesc);
		registry["LinearClampToEdge"] = LinearClampToEdge;

		// 가장 가까운 샘플러 (반복)

		samplerDesc.minFilter = FilterMode::Nearest;
		samplerDesc.magFilter = FilterMode::Nearest;
		samplerDesc.mipFilter = FilterMode::Nearest;
		samplerDesc.wrapU = WrapMode::Repeat;
		samplerDesc.wrapV = WrapMode::Repeat;
		samplerDesc.wrapW = WrapMode::Repeat;
		NearestRepeat = Sampler::Create(samplerDesc);
		registry["NearestRepeat"] = NearestRepeat;

		// 가장 가까운 샘플러 (클램프)

		samplerDesc.minFilter = FilterMode::Nearest;
		samplerDesc.magFilter = FilterMode::Nearest;
		samplerDesc.mipFilter = FilterMode::Nearest;
		samplerDesc.wrapU = WrapMode::ClampToEdge;
		samplerDesc.wrapV = WrapMode::ClampToEdge;
		samplerDesc.wrapW = WrapMode::ClampToEdge;
		NearestClampToEdge = Sampler::Create(samplerDesc);
		registry["NearestClampToEdge"] = NearestClampToEdge;

	}

}