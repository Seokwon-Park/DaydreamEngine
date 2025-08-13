#pragma once

#include "UIPanel.h"
#include "Daydream/Graphics/Resources/Framebuffer.h"

namespace Daydream
{
	class ViewportPanel : public UIPanel
	{
	public:
		ViewportPanel();
		~ViewportPanel() {};

		inline void SetFramebuffer(Shared<Framebuffer> _framebuffer) { viewportFramebuffer = _framebuffer; };

		virtual void OnImGuiRender() override;
	protected:

	private:
		Shared<Framebuffer> viewportFramebuffer;
	};
}
