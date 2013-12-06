#ifndef PLANE_H
#define PLANE_H

#include "Vector.h"
#include "Line.h"

// Plane class holds equation values as well as various
// functions for plane operations.
class Plane
{
public:
	Plane();
	Plane( const Vector& point, const Vector& normal );
	Plane( const Vector& point1, const Vector& point2, const Vector& point3 );
	~Plane();

	// point on plane intersected by line
	Vector lineIntersect( const Line& testLine ) const;
	// new plane when this one is rotated
	Plane getRotatedPlane( const Vector& axis, float angle ) const;

	Vector getPoint() const { return m_point; }
	Vector getNormal() const { return m_normal; }

private:
	// defined by a point and normal
	Vector m_point;
	Vector m_normal;
};

#endif