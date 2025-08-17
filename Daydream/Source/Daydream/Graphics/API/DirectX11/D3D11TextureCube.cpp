#include "DaydreamPCH.h"
#include "D3D11TextureCube.h"

#include "Daydream/Graphics/Utility/GraphicsUtil.h"
#include "Daydream/Graphics/Utility/DDSLoader.h"

namespace Daydream
{
	D3D11TextureCube::D3D11TextureCube(const FilePath& _path)
	{
		DDSLoader::LoadDDS(_path);
	}

}