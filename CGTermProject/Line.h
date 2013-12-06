#ifndef LINE_H
#define LINE_H

#include "Vector.h"

// util line class
class Line
{
public:
	Line();
	Line( const Vector& point, const Vector& direction );
	~Line();

	Vector getPoint() const { return m_point; }
	Vector getDirection() const { return m_direction; }

private:
	// line defined by point and direction
	Vector m_point;
	Vector m_direction;
};

#endif