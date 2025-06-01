#pragma once

#include <Steins.h>

class EditorLayer : public Steins::Layer
{
public:
	EditorLayer();

	virtual void OnImGuiRender() override;
};
