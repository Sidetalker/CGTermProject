#ifndef PLANE_H
#define PLANE_H

#include "Vector.h"
#include "Line.h"

// Plane class holds equation values as well as various
// functions for plane operations.

// TODO: continue implementing plane class
class Plane
{
public:
	Plane();
	Plane( const Vector& point, const Vector& normal );
	Plane( const Vector& point1, const Vector& point2, const Vector& point3 );
	~Plane();

	Vector vectorIntersect( const Line& testLine ) const;

private:
	Vector m_point;
	Vector m_normal;
};

#endif