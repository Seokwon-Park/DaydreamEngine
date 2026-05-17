#pragma once

#include "RenderGraph.h"
#include "Daydream/Graphics/Camera/Camera.h"

namespace Daydream
{
	class Scene;

    class SceneRenderer
    {
    public:
        SceneRenderer();
        virtual ~SceneRenderer();

        void Init();

        void RenderScene(Shared<Scene> _scene, Shared<Camera> _camera) {};

    private:
        Shared<RenderGraph> renderGraph;

        RenderGraphResourceHandle depthResource;
        RenderGraphResourceHandle gBufferResource;
        RenderGraphResourceHandle finalResource;
    };
}
