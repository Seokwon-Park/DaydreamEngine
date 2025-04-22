#include "SteinsPCH.h"
#include "D3D11Texture.h"

#include "stb_image.h"

namespace Steins
{
	D3D11Texture2D::D3D11Texture2D(const FilePath& _path)
	{
		//stbi_uc* data = stbi_load(_path.ToString().c_str());
	}
	D3D11Texture2D::~D3D11Texture2D()
	{
	}
	void D3D11Texture2D::Bind(UInt32 _slot) const
	{
	}
}
