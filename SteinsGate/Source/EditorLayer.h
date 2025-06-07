#pragma once

#include <Steins.h>

class EditorLayer : public Steins::Layer
{
public:
	EditorLayer();

	virtual void OnUpdate(Float32 _deltaTime) override;
	virtual void OnImGuiRender() override;
	void OpenDockspace();
private:
	Steins::Shared<Steins::VertexBuffer> squareVB;
	Steins::Shared<Steins::IndexBuffer> squareIB;

	Steins::Shared<Steins::Shader> vs;
	Steins::Shared<Steins::Shader> ps;

	Steins::Shared<Steins::PipelineState> pso;
	Steins::Shared<Steins::ConstantBuffer> viewProjMat;
	Steins::Shared<Steins::Texture2D> texture;

	Steins::Shared<Steins::Framebuffer> viewport;

	Steins::OrthographicCamera camera = Steins::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
	Steins::Matrix4x4 cameraPos;
};
