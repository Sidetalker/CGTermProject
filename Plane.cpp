#include "Plane.h"


Plane::Plane( const Vector& point, const Vector& normal ) :
  m_point( point )
, m_normal( normal.unit() )
{
}


Plane::~Plane()
{
}
