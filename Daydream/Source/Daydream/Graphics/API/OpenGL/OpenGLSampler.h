#pragma once

#include "Daydream/Graphics/Resources/Sampler.h"

namespace Daydream
{
	class OpenGLSampler : public Sampler
	{
	public:
		OpenGLSampler(const SamplerDesc& _desc);
		virtual ~OpenGLSampler();

		UInt32 GetSamplerID() const { return samplerID; }
	private:
		UInt32 samplerID;
	};
}
