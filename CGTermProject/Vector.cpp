#include "Vector.h"

#include <cmath>
#include <iostream>

static const float PI = 3.141592653589793238462643383279502884; // TODO: get const from somewhere else

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

// returns sum of two vectors
Vector Vector::operator+( const Vector &other ) const
{
	return Vector( m_x + other.m_x, m_y + other.m_y, m_z + other.m_z );
}

// returns difference of two vectors
Vector Vector::operator-( const Vector &other ) const
{
	return Vector( m_x - other.m_x, m_y - other.m_y, m_z - other.m_z );
}

// returns value of vector multiplied by a scalar
Vector Vector::operator*( float scalar ) const
{
	return Vector( m_x * scalar, m_y * scalar, m_z * scalar );
}

// returns cross product
Vector Vector::crossProduct( const Vector& other ) const
{
    return Vector(   ( ( m_y * other.m_z ) - ( other.m_y * m_z ) )
                   , ( ( other.m_x * m_z ) - ( m_x * other.m_z ) )
                   , ( ( m_x * other.m_y ) - ( other.m_x * m_y ) ) );
}

// returns dot product
float Vector::dotProduct( const Vector& other ) const
{
    return ( ( m_x * other.m_x ) + ( m_y * other.m_y ) + ( m_z * other.m_z ) );
}

// returns magnitude
float Vector::magnitude() const
{
    return sqrt( ( m_x * m_x ) + ( m_y * m_y ) + ( m_z * m_z ) );
}

// returns square magnitude for relative distance checking
float Vector::squareMagnitude() const
{
	return ( m_x * m_x ) + ( m_y * m_y ) + ( m_z * m_z );
}

// returns unit vector of length 1 and same direction
Vector Vector::unit() const
{
	return Vector( m_x / magnitude(), m_y / magnitude(), m_z / magnitude() );
}

// returns vector rotated about axis (through the origin) by an angle
Vector Vector::getRotatedVector( const Vector& axis, float angle ) const
{
	// make sure axis is unit vector
	const float angleRad = angle * PI / 180;
	const Vector a( axis.unit() );
	return ( a * a.dotProduct( *this ) ) + ( *this - ( a * a.dotProduct( *this ) ) ) * cos( angleRad ) + a.crossProduct( *this ) * sin( angleRad ); 
}

// prints debugging statement about vector coordinate values
void Vector::print() const
{
	std::cout << '(' << m_x << ", " << m_y << ", " << m_z << ")\n";
}