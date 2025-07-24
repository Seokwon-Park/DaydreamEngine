#include "SteinsPCH.h"
#include "Mesh.h"
#include <Steins/Graphics/Utility/MeshLoader.h>

namespace Steins
{
	void Mesh::Load(const FilePath& _filepath)
	{
		MeshLoader::LoadFromFile(_filepath);
	}
}