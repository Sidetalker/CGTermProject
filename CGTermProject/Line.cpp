#include "Line.h"

Line::Line()
{
}

Line::Line( const Vector& point, const Vector& direction ) :
  m_point( point )
, m_direction( direction )
{
}


Line::~Line()
{
}
