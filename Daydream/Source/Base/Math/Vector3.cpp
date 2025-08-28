#include "DaydreamPCH.h"
#include "Vector3.h"

namespace Daydream
{
	Vector3 RotateAxisXDegree(const Vector3& _vector, Float32 _degree)
	{
		return RotateAxisXRadian(_vector, Math::DegreeToRadian(_degree));
	}
	Vector3 RotateAxisYDegree(const Vector3& _vector, Float32 _degree)
	{
		return RotateAxisYRadian(_vector, Math::DegreeToRadian(_degree));
	}
	Vector3 RotateAxisZDegree(const Vector3& _vector, Float32 _degree)
	{
		return RotateAxisZRadian(_vector, Math::DegreeToRadian(_degree));
	}
	Vector3 RotateAxisXRadian(const Vector3& _vector, Float32 _radian)
	{
		Vector3 result = _vector;
		result.y = _vector.y * cosf(_radian) - _vector.z * sinf(_radian);
		result.z = _vector.y * sinf(_radian) + _vector.z * cosf(_radian);
		return result;
	}
	Vector3 RotateAxisYRadian(const Vector3& _vector, Float32 _radian)
	{
		Vector3 result = _vector;
		result.z = _vector.z * cosf(_radian) - _vector.x * sinf(_radian);
		result.x = _vector.z * sinf(_radian) + _vector.x * cosf(_radian);
		return result;
	}
	Vector3 RotateAxisZRadian(const Vector3& _vector, Float32 _radian)
	{
		Vector3 result = _vector;
		result.x = _vector.x * cosf(_radian) - _vector.y * sinf(_radian);
		result.y = _vector.x * sinf(_radian) + _vector.y * cosf(_radian);
		return result;
	}
}
