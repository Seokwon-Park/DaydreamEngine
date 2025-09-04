#pragma once

#include "Daydream.h"
#include "UIPanel.h"

namespace Daydream
{
	class AssetBrowserPanel : public UIPanel
	{
	public:
		AssetBrowserPanel();
		~AssetBrowserPanel();

		void OnImGuiRender() override;
	protected:

	private:
		Path currentPath;


	};
}
