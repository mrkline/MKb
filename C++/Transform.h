#pragma once

#include "Exceptions.h"
#include "Vector3.h"

/*!
\breif A basic transform class

Inspired by and borrowed heavily from Irrlicht's transform matrix. Uses floats.
*/
class Transform
{
public:
	/*!
	\brief Type of matrix to construct
	\see Transform(ConstructType)
	*/
	enum ConstructType
	{
		E_MT_EMPTY, //!< An empty matrix
		E_MT_IDENTITY //!< An identity matrix
	};

	/*!
	\brief Copy constructor
	\param other Transform to copy
	*/
	Transform(const Transform& other);

	/*!
	\brief Constructs a transform from the first 16 floats in an array
	\param matrixArray Array to construct the transform from.
	*/
	explicit Transform(const float* matrixArray);

	/*!
	\brief Default constructor
	\param type The type of transform to construct.
	\see ConstructType
	*/
	explicit Transform(ConstructType type = E_MT_IDENTITY);

	/*!
	\brief Sets a transform the inverse of this one, if possible
	\param out The transform to set to the inverse
	\return true on success, false if there is no inverse
	\throws MathException if no inverse exists

	The inverse is calculated using Cramers rule.
	*/
	void GetInverse(Transform& out) const;

	/*!
	\brief Returns a transform  that is the inverse of this one, if possible
	\return The inverse of this transform
	\throws MathException if no inverse exists

	The inverse is calculated using Cramers rule.
	*/
	Transform GetInverse() const
	{
		Transform temp;
		GetInverse(temp);
		return temp;
	}

	/*!
	\brief Sets the transform to its inverse, if possible
	\throws MathException if no inverse exists

	The inverse is calculated using Cramers rule.
	*/
	void SetToInverse()
	{
		Transform temp;
		GetInverse(temp);
		*this = temp;
	}

	/*!
	\brief Sets a transform ot the transpose of this one, if possible
	\param out The transform to set to the transpose
	*/
	void GetTransposed(Transform& out) const;

	/*!
	\brief Get a transform equal to this transform after being transposed
	\return The transpose of this transform
	*/
	Transform GetTransposed() const
	{
		Transform ret;
		GetTransposed(ret);
		return ret;
	}

	/*!
	\brief Interpolate between this transform and another
	\param other Interpolation will be between this transform and other
	\param t The interpolation factor (between 0 and 1)
	\param out The transform to set to the interpolation
	\todo Make this method static, as dot and cross in Vector3
	*/
	void Interpolate(const Transform& other, float t, Transform& out) const;

	/*!
	\brief Get a transform equal to this transform interpolated with another
	\param other Interpolation will be between this transform and other
	\param t The interpolation factor (between 0 and 1)
	\return The interpolation between this transform and other at t
	\todo Make this method static, as with dot and cross in Vector3
	*/
	Transform Interpolate(const Transform& other, float t)
	{
		Transform ret;
		Interpolate(other, t, ret);
		return ret;
	}

	/*!
	\brief Checks equality using Math::kFloatRoundError as tolerance
	\see Math::kFloatRoundError
	*/
	bool Equals(const Transform& other,
	            float roundingTolerance = Math::kFloatRoundError) const;

	//! Returns true if this transform is an identity matrix
	bool IsIdentity() const;

	//! Returns true if this transform is orthagonal
	bool IsOrthogonal() const;

	/*!
	\brief Sets a vector to the rotation of this transform in degrees
	\param vecOut Upon completion, vecOut contains the rotation in degrees
	*/
	void GetRotationDegrees(Vector3& vecOut) const;

	/*!
	\brief Gets the rotation of this transform in degrees
	\return The rotation of this transform in degrees
	*/
	Vector3 GetRotationDegrees() const
	{
		Vector3 ret;
		GetRotationDegrees(ret);
		return ret;
	}

	/*!
	\brief Sets a vector to the rotation of this transform in radians
	\param vecOut Upon completion, vecOut contains the rotation in radians
	*/
	void GetRotationRadians(Vector3& vecOut) const;

	/*!
	\brief Gets the rotation of this transform in radians
	\return The rotation of this transform in radians
	*/
	Vector3 GetRotationRadians() const
	{
		Vector3 ret;
		GetRotationRadians(ret);
		return ret;
	}

	/*!
	\brief Sets a vector to the scaling from this transform
	\param vecOut Upon completion, vecOut contains the scale of this transform

	Note that this always returns the absolute values of the scale components.
	Negative scales cannot be recovered.
	*/
	void GetScale(Vector3& vecOut) const;

	/*!
	\brief Gets the scale of this transform
	\return The absolute scale of the transform

	Note that this always returns the absolute values of the scale components.
	Negative scales cannot be recovered.
	*/
	Vector3 GetScale() const
	{
		Vector3 ret;
		GetScale(ret);
		return ret;
	}

	/*!
	\brief Sets a vector to the translation from this transform
	\param vecOut Upon completion, vecOut contains the translation of this transform
	*/
	void GetTranslation(Vector3& vecOut) const;

	/*!
	\brief Gets the translation of this transform
	\return The translation of this transform
	*/
	Vector3 GetTranslation() const
	{
		Vector3 ret;
		GetTranslation(ret);
		return ret;
	}

	/*!
	\brief Gets the 16-element (4 x 4) array that makes up this transfor matrix
	\return A pointer to the array
	*/
	float* GetArray() { return matrix; }

	/*!
	\brief Gets the 16-element (4 x 4) array that makes up this transfor matrix
	\return A pointer to the array
	*/
	const float* GetArray() const { return matrix; }

	//! Sets the transform to the identity transform
	void SetToIdentity();

	/*!
	\brief Sets the transform to a product of two other transforms
	\param t1 The first transform to multiply
	\param t2 The second transform to multiply
	*/
	void SetAsProductOf(const Transform& t1, const Transform& t2);

	//! Sets the rotation to the inverse of the provided rotation in degrees
	void SetInverseRotationDegrees(const Vector3& rotation);
	//! Sets the rotation to the inverse of the provided rotation in radians
	void SetInverseRotationRadians(const Vector3& rotation);
	//! Sets the translation to the inverse of the provided translation
	void SetInverseTranslation(const Vector3& translation);
	//! Sets the rotation of this transform to the provided one in degrees
	void SetRotationDegrees(const Vector3& rotation);
	//! Sets the rotation of this transform to the provided one in radians
	void SetRotationRadians(const Vector3& rotation);
	//! Sets the translation of this transform
	void SetTranslation(const Vector3& translation);
	//! Sets the scale of this transform
	void SetScale(const Vector3& rotation);
	//! Sets the transform from the first 16 values of an array
	void SetFromArray(const float* transformMatrix);

	//! Rotates a point using the inverse of this transform's rotation
	void InverseRotatePoint(Vector3& pointOut) const;
	//! Translates a point using the inverse of this transforms's translation
	void InverseTranslatePoint(Vector3& pointOut) const;
	//! Rotates a point using this transforms's rotation
	void RotatePoint(Vector3& pointOut) const;
	//! Translates a point using this transform's translation
	void TranslatePoint(Vector3& pointOut) const;
	//! Scales a point using this transform's scale
	void ScalePoint(Vector3& pointOut) const;
	//! Transforms a point using this transform
	void TransformPoint(Vector3& pointOut) const;

	/*!
	\brief Tests for equality, using Math::kFloatRoundError as tolerance
	\see Equals
	*/
	bool operator==(const Transform& other) const { return Equals(other); }

	/*!
	\brief Tests for inequality, using Math::kFloatRoundError as tolerance
	\see Equals
	*/
	bool operator!=(const Transform& other) const { return !Equals(other); }
	Transform operator*(const Transform& m2) const;
	Transform operator*(const float scalar) const;
	Transform& operator*=(const Transform& other);
	Transform& operator*=(float scalar);
	Transform operator+(const Transform& other) const;
	Transform& operator+=(const Transform& other);
	Transform operator-(const Transform& other) const;
	Transform& operator-=(const Transform& other);
	Transform& operator=(const Transform& other);

	//! Access a transform value by index
	float& operator[](unsigned int index) { return matrix[index]; }
	//! Access a transform value by index
	float operator[](unsigned int index) const { return matrix[index]; }
	//! Access a transform value by row and column
	float& operator()(unsigned int row, unsigned int col)
	{ return matrix[row * 4 + col]; }
	//! Access a transform value by row and column
	float operator()(unsigned int row, unsigned int col) const
	{ return matrix[row * 4 + col]; }

protected:
	//! The array of floats for the matrix
	float matrix[16];
};
