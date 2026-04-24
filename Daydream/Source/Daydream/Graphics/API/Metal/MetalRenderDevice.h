#pragma once
#include "Daydream/Graphics/Core/RenderDevice.h"

namespace Daydream
{
    class MetalRenderDevice : public RenderDevice
    {
        // Inherited via RenderDevice
        void Init() override;
        void Shutdown() override;
    };
}

