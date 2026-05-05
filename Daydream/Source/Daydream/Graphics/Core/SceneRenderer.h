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

        // 엔진 초기화 시점에 단 한 번 호출하여 RenderGraph의 "구조(Topology)"를 세팅합니다.
        void Init();

        // 매 프레임 호출되며, 씬 데이터와 카메라 정보를 받아 렌더링을 실행합니다.
        void RenderScene(Shared<Scene> _scene, Shared<Camera> _camera) {};

    private:
        RenderGraph m_RenderGraph;

        // RenderGraph 리소스 핸들 캐싱
        RenderGraphResourceHandle m_DepthResource;
        RenderGraphResourceHandle m_GBufferResource;
        RenderGraphResourceHandle m_FinalResource;
    };
}
