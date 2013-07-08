#ifndef __MK_LINE_2D_HPP__
#define __MK_LINE_2D_HPP__

#include "Vector2.hpp"

/// A line (segment) in 2D space
class Line2 {

public:
	/// Constructs the line from a start and end point
	/// \param s The starting point
	/// \param s The ending point
	Line2(const Vector2& s, Vector2& e) : start(s), end(e) { }

	/**
	 * \brief Constructs the line from a start and end point
	 * \param sx The x coordinate of the starting point
	 * \param sy The y coordinate of the starting point
	 * \param ex The x coordinate of the ending point
	 * \param ey The y coordinate of the ending point
	 */
	Line2(float sx, float sy, float ex, float ey) : start(sx, sy), end(ex, ey) { }

	/// Gets the middle of the line segment
	Vector2 getMiddle() const { return (end - start) / 2.0f; }

	float getLength() const { return (end - start).getLength(); }

	float getLengthSq() const { return (end - start).getLengthSq(); }

	/**
	 * \brief Gets the point on the line or line segment closest to p
	 * \param p The point of interest
	 * \param pastSegment true to extend the line past the segment endpoints
	 * \returns The point on the line closest to p
	 */
	Vector2 projectPoint(const Vector2& p, float pastSegment = false) const
	{
		const float l2 = getLengthSq();

		// If the line is length zero, just give back the point
		if (l2 == 0.0f)
			return start;

		// Consider the parameterized version of the line, start + t * direction
		// Calculate the t value for our desired point
		const Vector2 seg = end - start;
		const float t = Vector2::dot(p - start, seg) / l2;

		// If we don't want to look past our segment endpoints and the closest point is past them,
		// return an endpoint
		if (t < 0.0f && !pastSegment)
			return start;

		if (t > 1.0f && !pastSegment)
			return end;

		// Otherwise return our projected point
		return start + seg * t;
	}

	/**
	 * \brief Gets the squred distance from the line or line segment
	 * \param p The point of interest
	 * \param pastSegment true to extend the line past the segment endpoints
	 * \returns the squared distance of point p from the line
	 */
	float getDistanceSq(const Vector2& p, bool pastSegment = false) const
	{
		const Vector2 projection = projectPoint(p, pastSegment);
		return (p - projection).getLengthSq();
	}

	/**
	 * \brief Gets the distance from the line or line segment
	 * \param p The point of interest
	 * \param pastSegment true to extend the line past the segment endpoints
	 * \returns the distance of point p from the line
	 */
	float getDistance(const Vector2& p, bool pastSegment = false) const
	{ return std::sqrt(getDistanceSq(p, pastSegment)); }

	/*
	 * \brief Checks if a given point is one the line or line segment
	 * \param tolerance The tolerance (in ULPs) allowed between a and b for them to
	 *                  still be considered equal
	 * \returns true if point p is on the line within the given tolerance
	 */
	bool isOnLine(const Vector2& p, bool pastSegment = false, int tolerance = Math::kUlpsEquality) const
	{ return Math::isZero(getDistance(p, pastSegment), tolerance); }

	Vector2 start;
	Vector2 end;
};

#endif
