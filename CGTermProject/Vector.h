#ifndef VECTOR_H
#define VECTOR_H

// Vector class holds coordinate values as well as various
// functions for vector operations.
class Vector
{
public:
	Vector();
    Vector( float x, float y, float z );
	Vector( const Vector& other );
	~Vector();

	// operations
	Vector operator+( const Vector& other ) const;
	Vector operator-( const Vector& other ) const;
	Vector operator*( float scalar ) const;

	// vector operations
    Vector crossProduct( const Vector& other ) const;
    float dotProduct( const Vector& other ) const;
    float magnitude() const;
	float squareMagnitude() const;
	Vector unit() const;

	// new vector when this one is rotated
	Vector getRotatedVector( const Vector& axis, float angle ) const;

	float getX() const { return m_x; }
	float getY() const { return m_y; }
	float getZ() const { return m_z; }

	void print() const;

private:
    float m_x;  // x coordinate
    float m_y;  // y coordinate
    float m_z;  // z coordinate
};

#endif