#pragma once

#include "Steins/Render/Texture.h"

namespace Steins
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const FilePath& _path);
		virtual ~OpenGLTexture2D() override;

		virtual void Bind(UInt32 slot) const override;
	private:
		UInt32 rendererID;
	};
}