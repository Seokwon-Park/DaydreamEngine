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
		void RenderFolderTree(const Path& path, Path& selectedPath);
	protected:

	private:
		Float32 padding = 8.0f;
		Float32 thumbnailSize = 64.0f;

		Path currentPath;
		String draggedAssetString;
	};
}
