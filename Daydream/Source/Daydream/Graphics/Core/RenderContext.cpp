#include "DaydreamPCH.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Resources/PipelineState/GraphicsPipelineState.h"
#include "Daydream/Graphics/Manager/ResourceManager.h"

namespace Daydream
{
    void RenderContext::BindPipelineState(Shared<GraphicsPipelineState> _pipelineState)
    {
        activePipelineState = _pipelineState;
    }

    //void RenderContext::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
    //{
    //    //if (!_texture->HasSampler())
    //    //{
    //    //    _texture->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));
    //    //}
    //}
    void RenderContext::CaptureResource(Shared<GPUResource> _resource)
    {
        captureStorage.push_back(_resource);
    }
    void RenderContext::ReleaseCapturedBuffer()
    {
        captureStorage.clear();
    }
}
