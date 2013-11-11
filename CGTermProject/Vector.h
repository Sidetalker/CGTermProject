#ifndef VECTOR_H
#define VECTOR_H

// Vector class holds coordinate values as well as various
// functions for vector operations.

class Vector
{
public:
    Vector( float x, float y, float z );
	Vector( const Vector& other );
	~Vector();

	Vector operator+( const Vector& other ) const;
	Vector operator-( const Vector& other ) const;

    Vector crossProduct( const Vector& other ) const;
    float dotProduct( const Vector& other ) const;
    float magnitude() const;
	Vector unit() const;

	float getX() const { return m_x; }
	float getY() const { return m_y; }
	float getZ() const { return m_z; }

private:
    float m_x;  // x coordinate
    float m_y;  // y coordinate
    float m_z;  // z coordinate
};

#endif