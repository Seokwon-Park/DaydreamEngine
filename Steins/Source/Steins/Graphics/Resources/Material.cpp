#include "SteinsPCH.h"
#include "Material.h"

#include "Steins/Graphics/Core/Renderer.h"
#include "Steins/Graphics/Core/PipelineState.h"

namespace Steins
{
    Shared<Material> Material::Create(Shared<PipelineState> _pipeline)
    {
        return Renderer::GetRenderDevice()->CreateMaterial(_pipeline);
    }
}
