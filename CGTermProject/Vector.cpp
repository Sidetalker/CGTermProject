#include "Vector.h"

#include <cmath>

Vector::Vector() :
  m_x( 0 )
, m_y( 0 )
, m_z( 0 )
{
}

Vector::Vector( float x, float y, float z ) :
  m_x( x )
, m_y( y )
, m_z( z )
{
}

Vector::Vector( const Vector& other ) :
  m_x( other.m_x )
, m_y( other.m_y )
, m_z( other.m_z )
{
}

Vector::~Vector()
{
}

Vector Vector::operator+( const Vector &other ) const
{
	return Vector( m_x + other.m_x, m_y + other.m_y, m_z + other.m_z );
}

Vector Vector::operator-( const Vector &other ) const
{
	return Vector( m_x - other.m_x, m_y - other.m_y, m_z - other.m_z );
}

// Returns cross product
Vector Vector::crossProduct( const Vector& other ) const
{
    return Vector(   ( ( m_y * other.m_z ) - ( other.m_y * m_z ) )
                   , ( ( other.m_x * m_z ) - ( m_x * other.m_z ) )
                   , ( ( m_x * other.m_y ) - ( other.m_x * m_y ) ) );
}

// Returns dot product
float Vector::dotProduct( const Vector& other ) const
{
    return ( ( m_x * other.m_x ) + ( m_y * other.m_y ) + ( m_z * other.m_z ) );
}

// Returns magnitude
float Vector::magnitude() const
{
    return sqrt( ( m_x * m_x ) + ( m_y * m_y ) + ( m_z * m_z ) );
}

Vector Vector::unit() const
{
	return Vector( m_x / magnitude(), m_y / magnitude(), m_z / magnitude() );
}