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
        RenderGraph m_RenderGraph;

        // RenderGraph 리소스 핸들 캐싱
        RenderGraphResourceHandle m_DepthResource;
        RenderGraphResourceHandle m_GBufferResource;
        RenderGraphResourceHandle m_FinalResource;
    };
}
