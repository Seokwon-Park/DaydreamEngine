#include "DaydreamPCH.h"
#include "Sampler.h"

#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
    Shared<Sampler> Sampler::Create(const SamplerDesc& _desc)
    {
        return Renderer::GetRenderDevice()->CreateSampler(_desc);
    }
}
