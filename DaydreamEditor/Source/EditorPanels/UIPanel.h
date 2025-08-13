#pragma once

#include "imgui/imgui.h"

namespace Daydream
{
	class UIPanel
	{
	public:
		virtual ~UIPanel() = default;

		virtual void OnImGuiRender() = 0;
	private:
	};
}