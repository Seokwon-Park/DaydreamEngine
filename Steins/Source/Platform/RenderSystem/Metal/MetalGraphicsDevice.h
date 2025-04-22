#pragma once
#include "Steins/Render/GraphicsDevice.h"

namespace Steins
{
    class MetalGraphicsDevice : public GraphicsDevice
    {
        // Inherited via GraphicsDevice
        void Init() override;
        void Shutdown() override;
        void Render() override;
        virtual Shared<VertexBuffer> CreateVertexBuffer(Float32* _vertices, UInt32 _size) {};
    };
}

