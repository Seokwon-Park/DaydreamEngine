#pragma once

#include <Steins.h>

class Sandbox2D : public Steins::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnUpdate(Float32 _deltaTime) override;
	virtual void OnImGuiRender() override;

private:
	Steins::Shared<Steins::VertexBuffer> squareVB;
	Steins::Shared<Steins::VertexBuffer> squareVB2;
	Steins::Shared<Steins::IndexBuffer> squareIB;
	Steins::Shared<Steins::IndexBuffer> squareIB2;

	Steins::Shared<Steins::Shader> vs;
	Steins::Shared<Steins::Shader> ps;

	Steins::Shared<Steins::PipelineState> pso;
	Steins::Shared<Steins::RenderPass> renderPass;
	Steins::Shared<Steins::ConstantBuffer> viewProjMat;
	Steins::Shared<Steins::Texture2D> texture;

	Steins::Shared<Steins::Material> material;
	Steins::Shared<Steins::Framebuffer> framebuffer;

	//Steins::OrthographicCamera camera = Steins::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
	Steins::Camera camera = Steins::Camera();
	Steins::Matrix4x4 cameraPos;

};

