#include "SteinsPCH.h"
#include "Matrix4x4.h"

namespace Steins
{
	Matrix4x4 Matrix4x4::Translate(Matrix4x4 _matrix, Vector3 _translate)
	{
		glm::make_mat4(_matrix.values);
	}
}
