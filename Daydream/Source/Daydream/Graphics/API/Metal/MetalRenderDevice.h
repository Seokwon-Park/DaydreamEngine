#pragma once
#include "Daydream/Graphics/Core/RenderDevice.h"

namespace Daydream
{
    class MetalRenderDevice : public RenderDevice
    {
        // Inherited via RenderDevice
        void Init() override;
        void Shutdown() override;
        void Render() override;
        virtual Shared<VertexBuffer> CreateStaticVertexBuffer(Float32* _vertices, UInt32 _size) {};
    };
}

