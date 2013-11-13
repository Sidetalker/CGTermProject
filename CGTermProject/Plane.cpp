#include "Plane.h"

Plane::Plane()
{
}

Plane::Plane( const Vector& point, const Vector& normal ) :
  m_point( point )
, m_normal( normal )
{
}


Plane::~Plane()
{
}

// returns point on plane of line intersection
Vector Plane::vectorIntersect( const Line& testLine ) const
{
	float t = ( ( m_normal.getX() * ( testLine.getPoint().getX() - m_point.getX() ) ) +
	            ( m_normal.getY() * ( testLine.getPoint().getY() - m_point.getY() ) ) +
	            ( m_normal.getZ() * ( testLine.getPoint().getZ() - m_point.getZ() ) ) ) / -( ( m_normal.getX() * testLine.getDirection().getX() ) +
	                                                                                         ( m_normal.getY() * testLine.getDirection().getY() ) +
	                                                                                         ( m_normal.getZ() * testLine.getDirection().getZ() ) );

	return Vector( testLine.getPoint().getX() + ( t * testLine.getDirection().getX() ),
	               testLine.getPoint().getY() + ( t * testLine.getDirection().getY() ),
	               testLine.getPoint().getZ() + ( t * testLine.getDirection().getZ() ) );
}

// returns plane with rotated vector (rotated about axis (through the origin) by an angle) with the same point
Plane Plane::getRotatedPlane( const Vector& axis, float angle ) const
{
	return Plane( m_point, m_normal.getRotatedVector( axis, angle ) );
}