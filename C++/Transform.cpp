#include "Transform.h"

#include <cstring>

#include "Exceptions.h"

//! Used for very quickly setting the identity matrices
static const float kIdentityMatrix[16] = { 1, 0, 0, 0,
                                           0, 1, 0, 0,
                                           0, 0, 1, 0,
                                           0, 0, 0, 1
                                         };

Transform::Transform(ConstructType type)
{
	if (type == E_MT_IDENTITY)
		SetToIdentity();
	else
		memset(matrix, 0, sizeof(float) * 16);
}

Transform::Transform(const float* matrixArray)
{
	memcpy(matrix, matrixArray, sizeof(float) * 16);
}

Transform::Transform(const Transform& other)
{
	*this = other;
}

void Transform::GetInverse(Transform& out) const
{
	const Transform &m = *this;

	float d = (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) -
	          (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
	          (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)) +
	          (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) -
	          (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
	          (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0));

	if (Math::IsZero(d))
	{
		throw MathException("The provided transform has no inverse.",
		                    __FUNCTION__);
	}

	d = 1.0f / d;

	out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) +
	                 m(1, 2) * (m(2, 3) * m(3, 1) - m(2, 1) * m(3, 3)) +
	                 m(1, 3) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)));
	out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(0, 3) * m(3, 2)) +
	                 m(2, 2) * (m(0, 3) * m(3, 1) - m(0, 1) * m(3, 3)) +
	                 m(2, 3) * (m(0, 1) * m(3, 2) - m(0, 2) * m(3, 1)));
	out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) +
	                 m(3, 2) * (m(0, 3) * m(1, 1) - m(0, 1) * m(1, 3)) +
	                 m(3, 3) * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)));
	out(0, 3) = d * (m(0, 1) * (m(1, 3) * m(2, 2) - m(1, 2) * m(2, 3)) +
	                 m(0, 2) * (m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1)) +
	                 m(0, 3) * (m(1, 2) * m(2, 1) - m(1, 1) * m(2, 2)));
	out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) +
	                 m(1, 3) * (m(2, 2) * m(3, 0) - m(2, 0) * m(3, 2)) +
	                 m(1, 0) * (m(2, 3) * m(3, 2) - m(2, 2) * m(3, 3)));
	out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(0, 3) * m(3, 0)) +
	                 m(2, 3) * (m(0, 2) * m(3, 0) - m(0, 0) * m(3, 2)) +
	                 m(2, 0) * (m(0, 3) * m(3, 2) - m(0, 2) * m(3, 3)));
	out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) +
	                 m(3, 3) * (m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)) +
	                 m(3, 0) * (m(0, 3) * m(1, 2) - m(0, 2) * m(1, 3)));
	out(1, 3) = d * (m(0, 2) * (m(1, 3) * m(2, 0) - m(1, 0) * m(2, 3)) +
	                 m(0, 3) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
	                 m(0, 0) * (m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2)));
	out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0)) +
	                 m(1, 0) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
	                 m(1, 1) * (m(2, 3) * m(3, 0) - m(2, 0) * m(3, 3)));
	out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(0, 1) * m(3, 0)) +
	                 m(2, 0) * (m(0, 1) * m(3, 3) - m(0, 3) * m(3, 1)) +
	                 m(2, 1) * (m(0, 3) * m(3, 0) - m(0, 0) * m(3, 3)));
	out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) +
	                 m(3, 0) * (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) +
	                 m(3, 1) * (m(0, 3) * m(1, 0) - m(0, 0) * m(1, 3)));
	out(2, 3) = d * (m(0, 3) * (m(1, 1) * m(2, 0) - m(1, 0) * m(2, 1)) +
	                 m(0, 0) * (m(1, 3) * m(2, 1) - m(1, 1) * m(2, 3)) +
	                 m(0, 1) * (m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0)));
	out(3, 0) = d * (m(1, 0) * (m(2, 2) * m(3, 1) - m(2, 1) * m(3, 2)) +
	                 m(1, 1) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
	                 m(1, 2) * (m(2, 1) * m(3, 0) - m(2, 0) * m(3, 1)));
	out(3, 1) = d * (m(2, 0) * (m(0, 2) * m(3, 1) - m(0, 1) * m(3, 2)) +
	                 m(2, 1) * (m(0, 0) * m(3, 2) - m(0, 2) * m(3, 0)) +
	                 m(2, 2) * (m(0, 1) * m(3, 0) - m(0, 0) * m(3, 1)));
	out(3, 2) = d * (m(3, 0) * (m(0, 2) * m(1, 1) - m(0, 1) * m(1, 2)) +
	                 m(3, 1) * (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) +
	                 m(3, 2) * (m(0, 1) * m(1, 0) - m(0, 0) * m(1, 1)));
	out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) +
	                 m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) +
	                 m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)));
}

void Transform::GetTransposed(Transform& out) const
{
	out[ 0] = matrix[ 0];
	out[ 1] = matrix[ 4];
	out[ 2] = matrix[ 8];
	out[ 3] = matrix[12];

	out[ 4] = matrix[ 1];
	out[ 5] = matrix[ 5];
	out[ 6] = matrix[ 9];
	out[ 7] = matrix[13];

	out[ 8] = matrix[ 2];
	out[ 9] = matrix[ 6];
	out[10] = matrix[10];
	out[11] = matrix[14];

	out[12] = matrix[ 3];
	out[13] = matrix[ 7];
	out[14] = matrix[11];
	out[15] = matrix[15];
}

void Transform::Interpolate(const Transform& other, float t, Transform& out) const
{
	for (unsigned int c = 0; c < 16; ++c)
		out[c] = matrix[c] + (other[c] - matrix[c]) * t;
}

bool Transform::Equals(const Transform& other, float roundingTolerance) const
{
	for (unsigned int c = 0; c < 16; ++c)
	{
		if (!Math::Equals(matrix[c], other[c], roundingTolerance))
			return false;
	}

	return true;
}

bool Transform::IsIdentity() const
{
	for (unsigned int c = 0; c < 16; ++c)
	{
		if (!Math::Equals(matrix[c], kIdentityMatrix[c]))
			return false;
	}
	return true;
}

bool Transform::IsOrthogonal() const
{
	float dp=matrix[0] * matrix[4] + matrix[1] * matrix[5] + matrix[2]
	         * matrix[6] + matrix[3] * matrix[7];


	if (!Math::IsZero(dp))
		return false;

	dp = matrix[0] * matrix[8] + matrix[1] * matrix[9] + matrix[2]
	     * matrix[10] + matrix[3] * matrix[11];

	if (!Math::IsZero(dp))
		return false;

	dp = matrix[0] * matrix[12] + matrix[1] * matrix[13] + matrix[2]
	     * matrix[14] + matrix[3] * matrix[15];
	if (!Math::IsZero(dp))
		return false;
	dp = matrix[4] * matrix[8] + matrix[5] * matrix[9] + matrix[6]
	     * matrix[10] + matrix[7] * matrix[11];

	if (!Math::IsZero(dp))
		return false;

	dp = matrix[4] * matrix[12] + matrix[5] * matrix[13] + matrix[6]
	     * matrix[14] + matrix[7] * matrix[15];

	if (!Math::IsZero(dp))
		return false;

	dp = matrix[8] * matrix[12] + matrix[9] * matrix[13] + matrix[10]
	     * matrix[14] + matrix[11] * matrix[15];

	return (Math::IsZero(dp));
}

void Transform::GetRotationRadians(Vector3& vecOut) const
{
	// Original (Irrlicht) code used 64-bit floats.

	Vector3 scale;
	GetScale(scale);
	const Vector3 invScale(1.0f / scale.X, 1.0f / scale.Y, 1.0f / scale.Z);

	// was 64-bit
	float Y = -asin(matrix[2]*invScale.X);
	// was 64-bit
	const float C = cos(Y);

	float rotx, roty, X, Z;

	if (!Math::IsZero(C))
	{
		const float invC = 1.0f / C;
		rotx = matrix[10] * invC * invScale.Z;
		roty = matrix[6] * invC * invScale.Y;
		X = atan2( roty, rotx );
		rotx = matrix[0] * invC * invScale.X;
		roty = matrix[1] * invC * invScale.X;
		Z = atan2( roty, rotx );
	}
	else
	{
		X = 0.0;
		rotx = matrix[5] * invScale.Y;
		roty = -matrix[4] * invScale.Y;
		Z = atan2( roty, rotx );
	}

	// fix values that get below zero
	// before it would set (!) values to 360
	// that were above 360:
	if (X < 0.0)
		X += 2 * Math::kPi;
	if (Y < 0.0)
		Y += 2 * Math::kPi;
	if (Z < 0.0)
		Z += 2 * Math::kPi;

}

void Transform::GetRotationDegrees(Vector3& vecOut) const
{
	GetRotationRadians(vecOut);
	vecOut.Scale(Math::kRadToDeg);
}

void Transform::GetScale(Vector3& vecOut) const
{
	// See http://www.robertblum.com/articles/2005/02/14/decomposing-matrices

	// Deal with the 0 rotation case first
	if (Math::IsZero(matrix[1]) && Math::IsZero(matrix[2]) &&
	        Math::IsZero(matrix[4]) && Math::IsZero(matrix[6]) &&
	        Math::IsZero(matrix[8]) && Math::IsZero(matrix[9]))
	{
		vecOut.Set(matrix[0], matrix[5], matrix[10]);
	}
	else
	{
		// We have to do the full calculation.
		vecOut.Set(sqrtf(matrix[0] * matrix[0] + matrix[1] * matrix[1] + matrix[2] * matrix[2]),
		           sqrtf(matrix[4] * matrix[4] + matrix[5] * matrix[5] + matrix[6] * matrix[6]),
		           sqrtf(matrix[8] * matrix[8] + matrix[9] * matrix[9] + matrix[10] * matrix[10]));
	}
}

void Transform::GetTranslation(Vector3& vecOut) const
{
	vecOut.Set(matrix[12], matrix[13], matrix[14]);
}

void Transform::SetToIdentity()
{
	memcpy(matrix, kIdentityMatrix, sizeof(float) * 16);
}

void Transform::SetAsProductOf(const Transform& t1, const Transform& t2)
{
	const float* m1 = t1.matrix;
	const float* m2 = t2.matrix;

	matrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	matrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	matrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	matrix[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

	matrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	matrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	matrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	matrix[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

	matrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	matrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	matrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	matrix[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

	matrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	matrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	matrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	matrix[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
}

void Transform::SetInverseRotationRadians(const Vector3& rotation)
{
	float cr = cos( rotation.X );
	float sr = sin( rotation.X );
	float cp = cos( rotation.Y );
	float sp = sin( rotation.Y );
	float cy = cos( rotation.Z );
	float sy = sin( rotation.Z );

	matrix[0] = cp * cy;
	matrix[4] = cp * sy;
	matrix[8] = -sp;

	float srsp = sr * sp;
	float crsp = cr * sp;

	matrix[1] = srsp * cy - cr * sy;
	matrix[5] = srsp * sy + cr * cy;
	matrix[9] = sr * cp;

	matrix[2] = crsp * cy + sr * sy;
	matrix[6] = crsp * sy - sr * cy;
	matrix[10] = cr*cp;
}

void Transform::SetInverseRotationDegrees(const Vector3& rotation)
{
	SetInverseRotationRadians(rotation.GetScaledBy(Math::kDegToRad));
}

void Transform::SetInverseTranslation(const Vector3& translation)
{
	matrix[12] = -translation.X;
	matrix[13] = -translation.Y;
	matrix[14] = -translation.Z;
}

void Transform::SetRotationRadians(const Vector3& rotation)
{
	const float cr = cos( rotation.X );
	const float sr = sin( rotation.X );
	const float cp = cos( rotation.Y );
	const float sp = sin( rotation.Y );
	const float cy = cos( rotation.Z );
	const float sy = sin( rotation.Z );

	matrix[0] = cp * cy;
	matrix[1] = cp * sy;
	matrix[2] = -sp;

	const float srsp = sr * sp;
	const float crsp = cr * sp;

	matrix[4] = srsp * cy - cr * sy;
	matrix[5] = srsp * sy + cr * cy;
	matrix[6] = sr * cp;

	matrix[8] = crsp * cy + sr * sy;
	matrix[9] = crsp * sy - sr * cy;
	matrix[10] = cr * cp;
}

void Transform::SetRotationDegrees(const Vector3& rotation)
{
	SetRotationRadians(rotation.GetScaledBy(Math::kDegToRad));
}

void Transform::SetTranslation(const Vector3& translation)
{
	matrix[12] = translation.X;
	matrix[13] = translation.Y;
	matrix[14] = translation.Z;
}

void Transform::SetScale(const Vector3& scale)
{
	matrix[0] = scale.X;
	matrix[5] = scale.Y;
	matrix[10] = scale.Z;
}

void Transform::SetFromArray(const float* transformMatrix)
{
	memcpy(matrix, transformMatrix, sizeof(float) * 16);
}

void Transform::InverseRotatePoint(Vector3& point) const
{
	Vector3& tmp = point;
	point.X = tmp.X * matrix[0] + tmp.Y * matrix[1] + tmp.Z *matrix[2];
	point.Y = tmp.X * matrix[4] + tmp.Y * matrix[5] + tmp.Z * matrix[6];
	point.Z = tmp.X * matrix[8] + tmp.Y * matrix[9] + tmp.Z * matrix[10];
}

void Transform::InverseTranslatePoint(Vector3& point) const
{
	point.X = point.X - matrix[12];
	point.Y = point.Y - matrix[13];
	point.Z = point.Z - matrix[14];
}

void Transform::RotatePoint(Vector3& point) const
{
	Vector3 tmp(point);
	point.X = tmp.X * matrix[0] + tmp.Y * matrix[4] + tmp.Z * matrix[8];
	point.Y = tmp.X * matrix[1] + tmp.Y * matrix[5] + tmp.Z * matrix[9];
	point.Z = tmp.X * matrix[2] + tmp.Y * matrix[6] + tmp.Z * matrix[10];
}

void Transform::TranslatePoint(Vector3& point) const
{
	point.X = point.X + matrix[12];
	point.Y = point.Y + matrix[13];
	point.Z = point.Z + matrix[14];
}

void Transform::ScalePoint(Vector3& point) const
{
	Vector3 scale;
	GetScale(scale);
	point.Scale(scale);
}

void Transform::TransformPoint(Vector3& point) const
{
	float vector[3];

	vector[0] = point.X * matrix[0] + point.Y * matrix[4]
	            + point.Z * matrix[8] + matrix[12];
	vector[1] = point.X * matrix[1] + point.Y * matrix[5]
	            + point.Z * matrix[9] + matrix[13];
	vector[2] = point.X * matrix[2] + point.Y * matrix[6]
	            + point.Z * matrix[10] + matrix[14];

	point.X = vector[0];
	point.Y = vector[1];
	point.Z = vector[2];
}

Transform Transform::operator*(const Transform& m2) const
{
	Transform m3(E_MT_EMPTY);
	m3.SetAsProductOf(*this, m2);
	return m3;
}

Transform Transform::operator*(const float scalar) const
{
	Transform ret(matrix);
	float* arr = ret.matrix;

	for (unsigned int c = 0; c < 16; ++c)
		arr[c] *= scalar;

	return ret;
}

Transform& Transform::operator*=(const Transform& other)
{
	Transform temp(matrix);
	SetAsProductOf(temp, other);
	return *this;
}

Transform& Transform::operator*=(const float scalar)
{
	for (unsigned int c = 0; c < 16; ++c)
		matrix[c] *= scalar;

	return *this;
}

Transform Transform::operator+(const Transform& other) const
{
	const float* otherMat = other.matrix;

	Transform ret(matrix);

	for (unsigned int c = 0; c < 16; ++c)
		ret.matrix[c] += otherMat[c];

	return ret;
}

Transform& Transform::operator+=(const Transform& other)
{
	const float* otherMat = other.matrix;

	for (unsigned int c = 0; c < 16; ++c)
		matrix[c] += otherMat[c];

	return *this;
}

Transform Transform::operator-(const Transform& other) const
{
	const float* otherMat = other.matrix;

	Transform ret(matrix);

	for (unsigned int c = 0; c < 16; ++c)
		ret.matrix[c] -= otherMat[c];

	return ret;
}

Transform& Transform::operator-=(const Transform& other)
{
	const float* otherMat = other.matrix;

	for (unsigned int c = 0; c < 16; ++c)
		matrix[c] -= otherMat[c];

	return *this;
}

Transform& Transform::operator=(const Transform& other)
{
	memcpy(matrix, other.matrix, sizeof(float) * 16);
	return *this;
}
