#include "DaydreamPCH.h"
#include "Material.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Core/PipelineState.h"

namespace Daydream
{
    Shared<Material> Material::Create(Shared<PipelineState> _pipeline)
    {
        return Renderer::GetRenderDevice()->CreateMaterial(_pipeline);
    }
}
