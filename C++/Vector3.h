#pragma once

#include <string>

#include "MKMath.h"

//! A three-dimensional vector using floats for each dimension
class Vector3
{
public:
	float X;
	float Y;
	float Z;

	//! Initializes vector to zero
	Vector3() : X(0.0f), Y(0.0f), Z(0.0f) {} 

	//! Initializes vector to provided x, y, and z values
	Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

	//! Initializes x, y, and z values to v
	explicit Vector3(float v) : X(v), Y(v), Z(v) {}

	//! Initializes vector with a provided vector's values
	Vector3(const Vector3& o) : X(o.X), Y(o.Y), Z(o.Z) {}

	//! Initializes vector with the first three values in the provided array
	explicit Vector3(float* arr) : X(arr[0]), Y(arr[1]), Z(arr[2]) {}
	
	Vector3 operator-() const { return Vector3(-X, -Y, -Z); }
	
	Vector3& operator=(const Vector3& o)
		{ X = o.X; Y = o.Y; Z = o.Z; return *this; }

	Vector3 operator+(const Vector3& o) const
		{ return Vector3(X + o.X, Y + o.Y, Z + o.Z); }
	Vector3& operator +=(const Vector3& o)
		{ X += o.X; Y += o.Y; Z += o.Z; return *this; }
	Vector3 operator+(float v) const
		{ return Vector3(X + v, Y + v, Z + v); }
	Vector3& operator+=(float v)
		{ X += v; Y += v; Z += v; return *this; }

	Vector3 operator-(const Vector3& o) const
		{ return Vector3(X - o.X, Y - o.Y, Z - o.Z); }
	Vector3& operator -=(const Vector3& o)
		{ X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
	Vector3 operator-(float v) const
		{ return Vector3(X - v, Y - v, Z - v); }
	Vector3& operator-=(float v)
		{ X -= v; Y -= v; Z -= v; return *this; }

	//! Comparison operators can be used to sort vectors with respect to X,
	//! then Y, then Z
	bool operator<=(const Vector3& o) const
	{
		return 	(X < o.X || Math::Equals(X, o.X)) ||
				(Math::Equals(X, o.X) && (Y < o.Y || Math::Equals(Y, o.Y))) ||
				(Math::Equals(X, o.X) && Math::Equals(Y, o.Y)
				 	&& (Z < o.Z || Math::Equals(Z, o.Z)));
	}

	//! Comparison operators can be used to sort vectors with respect to X,
	//! then Y, then Z
	bool operator>=(const Vector3& o) const
	{
		return 	(X > o.X || Math::Equals(X, o.X)) ||
				(Math::Equals(X, o.X) && (Y > o.Y || Math::Equals(Y, o.Y))) ||
				(Math::Equals(X, o.X) && Math::Equals(Y, o.Y)
				 	&& (Z > o.Z || Math::Equals(Z, o.Z)));
	}

	//! Comparison operators can be used to sort vectors with respect to X,
	//! then Y, then Z
	bool operator<(const Vector3& o) const
	{
		return 	(X < o.X && !Math::Equals(X, o.X)) ||
				(Math::Equals(X, o.X) && Y < o.Y && !Math::Equals(Y, o.Y)) ||
				(Math::Equals(X, o.X) && Math::Equals(Y, o.Y) && Z < o.Z
					&& !Math::Equals(Z, o.Z));
	}

	//! Comparison operators can be used to sort vectors with respect to X,
	//! then Y, then Z
	bool operator>(const Vector3& o) const
	{
		return 	(X > o.X && !Math::Equals(X, o.X)) ||
				(Math::Equals(X, o.X) && Y > o.Y && !Math::Equals(Y, o.Y)) ||
				(Math::Equals(X, o.X) && Math::Equals(Y, o.Y) && Z > o.Z
					&& !Math::Equals(Z, o.Z));
	}

	/*!
	\brief Checks equality using Math::kFloatRoundError as tolerance
	\see Math::kFloatRoundError
	*/
	bool operator==(const Vector3& o) const { return IsWithinTolerance(o); }
	/*!
	\brief Checks inequality using Math::kFloatRoundError as tolerance
	\see Math::kFloatRoundError
	*/
	bool operator!=(const Vector3& o) const { return !IsWithinTolerance(o); }

	/*
	\brief Checks if another vector is equal to this one within a
			provided tolerance
	\param o The other vector
	\param tolerance The tolerance allowed for each component of the
						two vectors to be within and still be considered equal
	\return True if this vector and o are equal within tolerance
	\see Math::kFloatRoundError
	*/
	bool IsWithinTolerance(const Vector3& o,
			float tolerance = Math::kFloatRoundError) const
	{
		return Math::Equals(X, o.X, tolerance)
			&& Math::Equals(Y, o.Y, tolerance)
			&& Math::Equals(Z, o.Z, tolerance);
	}

	//! Gets the length of this vector
	float GetLength() const { return std::sqrt(X*X + Y*Y + Z*Z); }

	//! Gets the length squared of this vector,
	//! which is faster to calculate than the length
	float GetLengthSq() const { return X*X + Y*Y + Z*Z; }

	//! Gets the distance from this vector to another one,
	//! interpreting both vectors as points
	float GetDistanceFrom(const Vector3& o) const
	{
		float dx, dy, dz;
		dx = X - o.X;
		dy = Y - o.Y;
		dz = Z - o.Z;
		return std::sqrt(dx*dx + dy*dy + dz*dz);
	}

	//! Gets the distance squared from this vector to another one,
	//! interpreting both vectors as points.
	//! This is faster to calculate than the distance itself.
	float GetDistanceSqFrom(const Vector3& o) const
	{
		float dx, dy, dz;
		dx = X - o.X;
		dy = Y - o.Y;
		dz = Z - o.Z;
		return dx*dx + dy*dy + dz*dz;
	}

	//! Returns true if this vector is a unit vector (with a length of 1)
	bool IsNormalized() const { return Math::Equals(std::sqrt(X*X + Y*Y + Z*Z), 1.0f); }

	//! Copies this vector into the first three values of the provided array
	void GetAsArray(float* arr) const
	{
		arr[0] = X;
		arr[1] = Y;
		arr[3] = Z;
	}

	//! Sets this vector to the provided values
	void Set(float x, float y, float z) { X = x; Y = y; Z = z; }

	//! Sets this vector's values from the first three values of the
	//! provided array
	void SetFromArray(float* asArray) { X = asArray[0]; Y = asArray[1]; Z = asArray[2]; }

	//! Set's vector's components to their mulitplicative inverses
	void SetToInverse() { X = 1.0f / X; Y = 1.0f / Y; Z = 1.0f / Z; }

	//! Gets an array with components (1/x, 1/y, 1/z) of this vector
	Vector3 GetInverse() const
		{ Vector3 ret(*this); ret.SetToInverse(); return ret; }

	//! Scales this vector by the components of the provided vector
	void Scale(const Vector3& o)
		{ X *= o.X; Y *= o.Y; Z *= o.Z; }

	//! Returns a copy of this vector, scaled by the provided vector
	Vector3 GetScaledBy(const Vector3& o) const { Vector3 ret(*this); ret.Scale(o); return ret; }

	//! Scales this vector by a provided scalar
	void Scale(float v)
		{ X *= v; Y *= v; Z *= v; }

	//! Returns a copy of this vector, scaled by the provided scalar
	Vector3 GetScaledBy(float v) const
		{ Vector3 ret(*this); ret.Scale(v); return ret; }

	//! Sets the length of this vector to 1
	void Normalize()
	{
		float len = std::sqrt(X*X + Y*Y + Z*Z);
		
		// Normalized already if our length is zero.
		// Also stops NaN errors
		if(Math::IsZero(len))
			return;

		X /= len;
		Y /= len;
		Z /= len;
	}

	//! Returns a copy of this vector with a length of 1
	Vector3 GetNormalized() const
		{ Vector3 ret(*this); ret.Normalize(); return ret; }

	//! Sets the length of this vector to a provided scalar
	void SetLength(float len)
	{
		Normalize();
		Scale(len);
	}

	//! Returns a copy of this vector with a length of the provided scalar
	Vector3 SetLength(float len) const
		{ Vector3 ret(*this); ret.SetLength(len); return ret; }

	/*!
	\brief Calculates the dot product of two vectors
	\param a The first vector in the dot product
	\param b The second vector in the dot product
	\return a dot b
	*/
	static float DotProduct(const Vector3& a, const Vector3& b)
	{
		return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
	}

	/*!
	\brief Calculates the cross product of two vectors
	\param a The first vector in the cross product
	\param b The second vector in the cross product
	\return a x b
	*/
	static Vector3 CrossProduct(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z,
				a.X * b.Y - a.Y * b.X);
	}

	//! Gets the left world vector (-1, 0, 0)
	static const Vector3& GetLeft()
	{
		static Vector3 left(-1.0f, 0.0f, 0.0f);
		return left;
	}

	//! Gets the right world vector, (1, 0, 0)
	static const Vector3& GetRight()
	{
		static Vector3 right(1.0f, 0.0f, 0.0f);
		return right;
	}

	//! Gets the forward world vector, (0, 0, 1)
	static const Vector3& GetForward()
	{
		static Vector3 forward(0.0f, 0.0f, 1.0f);
		return forward;
	}

	//! Gets the back world vector, (0, 0, -1)
	static const Vector3& GetBack()
	{
		static Vector3 back(0.0f, 0.0f, -1.0f);
		return back;
	}

	//! Gets the up world vector, (0, 1, 0)
	static const Vector3& GetUp()
	{
		static Vector3 up(0.0f, 1.0f, 0.0f);
		return up;
	}

	//! Gets the down world vector, (0, -1, 0)
	static const Vector3& GetDown()
	{
		static Vector3 down(0.0f, -1.0f, 0.0f);
		return down;
	}

	//! Gets (0, 0, 0)
	static const Vector3& GetZero()
	{
		static Vector3 zero(0.0f);
		return zero;
	}

	//! Gets (1, 1, 1)
	static const Vector3& GetOne()
	{
		static Vector3 one(1.0f);
		return one;
	}
};
