#pragma once

#include "Daydream/Graphics/Resources/Framebuffer.h"

namespace Daydream
{
	class Scene;

	class SceneRenderer
	{
	public:
		SceneRenderer();
		~SceneRenderer();

	//	// 에디터 뷰포트 크기가 변할 때 호출되어 내부 프레임버퍼를 리사이즈합니다.
	//	void SetViewportSize(UInt32 _width, UInt32 _height);

	//	// 씬 렌더링을 시작하며, 카메라의 ViewProjection 행렬 등을 세팅합니다.
	//	void BeginScene(const Scene* _scene, const Camera& _camera, const glm::mat4& _cameraTransform);

	//	// 실제 렌더 큐에 명령들을 밀어넣고 렌더링을 완료합니다.
	//	void EndScene();

	//	// 뷰포트 창에 띄우거나 마우스 피킹에 사용할 내부 프레임버퍼 반환
	//	Shared<Framebuffer> GetTargetFramebuffer() const { return m_SceneFramebuffer; }

	//private:
	//	// 내부 렌더 패스들
	//	void ShadowPass();
	//	void GeometryPass(); // Opaque 객체 및 Entity ID 렌더링
	//	void SkyboxPass();
	//	void PostProcessingPass();

	//private:
	//	const Scene* m_ActiveScene = nullptr;

	//	// 씬 렌더러가 독립적으로 소유하는 도화지
	//	Shared<Framebuffer> m_SceneFramebuffer;

	//	// 렌더링에 필요한 글로벌 데이터 (카메라 행렬, 환경광 데이터 등)
	//	struct SceneData
	//	{
	//		glm::mat4 ViewProjection;
	//		glm::vec3 CameraPosition;
	//	} m_SceneData;
	};
}
