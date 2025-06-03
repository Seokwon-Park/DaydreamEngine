#pragma once

#include <Steins.h>

class EditorLayer : public Steins::Layer
{
public:
	EditorLayer();

	virtual void OnUpdate(Float32 _deltaTime) override;
	virtual void OnImGuiRender() override;
};
