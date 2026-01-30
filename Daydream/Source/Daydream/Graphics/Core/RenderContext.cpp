#include "DaydreamPCH.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Resources/PipelineState.h"
#include "Daydream/Graphics/Manager/ResourceManager.h"

namespace Daydream
{
    void RenderContext::BindPipelineState(Shared<PipelineState> _pipelineState)
    {
        currentPipelineState = _pipelineState;
    }

    void RenderContext::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
    {
        if (!_texture->HasSampler())
        {
            _texture->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));
        }
    }
}
