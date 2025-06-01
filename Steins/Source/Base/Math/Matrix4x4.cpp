#include "SteinsPCH.h"
#include "Matrix4x4.h"

#include "MathUtil.h"

namespace Steins
{
	Matrix4x4::Matrix4x4()
	{
		glmMat = glm::mat4(1.0f);
	}
	void Matrix4x4::MatrixInverse()
	{
		glmMat = glm::inverse(glmMat);
		
	}
	void Matrix4x4::MatrixTranslate(Vector3 _translate)
	{
		//row_major -> col
		glm::transpose(glmMat);
		//col_major 기반연산
		glmMat = glm::translate(glmMat, _translate);
		//col_major -> row
		glm::transpose(glmMat);
	}
	void Matrix4x4::MatrixTranslate(Vector4 _translate)
	{
		
	}
	void Matrix4x4::MatrixTranspose()
	{
		glmMat = glm::transpose(glmMat);
	}

	Matrix4x4 Matrix4x4::operator*(Matrix4x4 _matrix)
	{
		glmMat *= _matrix.glmMat;
		return *this;
	}

	Vector4 Matrix4x4::operator*(Vector4 _vector) const
	{
		return glmMat * _vector;
	}

	Matrix4x4 Matrix4x4::Translate(Matrix4x4 _matrix, Vector3 _translate)
	{
		_matrix.MatrixTranslate(_translate);
		return _matrix;
	}
	Matrix4x4 Matrix4x4::Orthographic(float _left, float _right, float _bottom, float _top)
	{
		Matrix4x4 out; 
		out.glmMat = glm::ortho(_left, _right, _bottom, _top);
		out.MatrixTranspose(); // row-major;
		return out;
	}
	Matrix4x4 Matrix4x4::Inverse(Matrix4x4 _matrix)
	{
		_matrix.MatrixInverse();
		return _matrix;
	}
}
