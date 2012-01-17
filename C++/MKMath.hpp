#pragma once

#include <algorithm>
#include <cmath>

/*!
\brief Contains constants and functions for common math operations.

Operations already contained in the C++ STL are not re-implemented here.
The STL should be used  directly for such operations.
*/
namespace Math
{
	//! Float representation of Pi
	const float kPi		= 3.14159265359f;
	//! Float representation of 1 / Pi
	const float kPiRecip	= 1.0f / kPi;
	//! Float representation of Pi / 2
	const float kHalfPi	= kPi / 2.0f;
	//! Double representation of Pi
	const double kPiDouble		= 3.1415926535897932384626433832795028841971693993751;
	//! Double representation of 1 / Pi
	const double kPiDoubleRecip = 1.0 / kPiDouble;
	//! Double represntation of Pi / 2
	const double kHalfPiDouble = kPiDouble / 2.0;
	//! Conversion ratio to convert angle measurements in
	//! degrees to radian angle mesurements
	const float kDegToRad = kPi / 180.0f;
	//! Conversion ratio to convert angle measurements in
	//! radians to degree angle mesurements
	const float kRadToDeg   = 180.0f / kPi;
	//! Conversion ratio to convert angle measurements in
	//! degrees to radian angle mesurements
	const double kDegToRadDouble = kPiDouble / 180.0;
	//! Conversion ratio to convert angle measurements in
	//! radians to degree angle mesurements
	const double kRadToDegDouble = 180.0 / kPiDouble;
	//! Rounding error of a float
	//! \todo Get a more "real" value? Such as from std library?
	const float kFloatRoundError = 0.1f;
	//! Rounding error of a double
	//! \todo Get a more "real" value? Such as from std library?
	const double kDoubleRoundError = 0.01;

	/*!
	\brief Clamps a value between a low and high value
	\param value The value to clamp
	\param low The minimum return value
	\param high The maximum return value
	\return low <= return value <= high

	Clamps a value between a low and high value using std::min and std::max
	*/
	template <class T>
	inline const T Clamp (const T& value, const T& low, const T& high)
	{
		return std::min(std::max(value, low), high);
	}

	/*!
	\brief Tests two floats for equality within a given tolerance
	\param a The first value to test for equality
	\param b The second value to test for equality
	\param tolerance The tolerance allowed between a and b for them to
			still be considered equal
	\return a == b, within tolerance

	This allows for safer floating comparisons,
	since direct ones may work poorly due to rounding error.
	*/
	inline bool Equals(float a, float b, float tolerance = kFloatRoundError)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

	/*!
	\brief Tests two doubles for equality within a given tolerance
	\param a The first value to test for equality
	\param b The second value to test for equality
	\param tolerance The tolerance allowed between a and b for them to
			still be considered equal
	\return a == b, within tolerance

	This allows for safer floating comparisons,
	since direct ones may work poorly due to rounding error.
	*/
	inline bool Equals(double a, double b, double tolerance = kDoubleRoundError)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

	/*!
	\brief Tests a float for equality to zero
	\param a The first value to test for equality to zero
	\param tolerance The tolerance allowed between 0 and a for a to
			still be considered 0
	\return a == 0.0f, within tolerance
	\see Equals(float, float, float)

	This allows for safer floating comparisons,
	since direct ones may work poorly due to rounding error.
	*/
	inline bool IsZero(float a, float tolerance = kFloatRoundError)
	{
		return (a + tolerance >= 0.0f) && (a - tolerance <= 0.0f);
	}

	/*!
	\brief Tests a double for equality to zero
	\param a The first value to test for equality to zero
	\param tolerance The tolerance allowed between 0 and a for a to
			still be considered 0
	\return a == 0.0, within tolerance
	\see Equals(double, double, double)

	This allows for safer floating comparisons,
	since direct ones may work poorly due to rounding error.
	*/
	inline bool IsZero(double a, double tolerance = kDoubleRoundError)
	{
		return (a + tolerance >= 0.0) && (a - tolerance <= 0.0);
	}

} // end namespace Math
