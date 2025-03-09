#pragma once

namespace Steins
{
    class RenderSystem
    {
    public:
        virtual ~RenderSystem() {};

        virtual void Init() = 0;  
        virtual void Shutdown() = 0;
        virtual void Render() = 0;  
        virtual void SwapBuffers() = 0;
    protected:
    };
}
