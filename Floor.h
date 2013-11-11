#ifndef FLOOR_H
#define FLOOR_H

// TODO: implement floor to be drawn
class Floor
{
public:
	Floor( float x = 0, float z = 0, float squareSize = 5.0, float width = 50.0, float depth = 100.0 );
	~Floor(void);

	void draw();

private:
	float m_squareSize;
	float m_width;
	float m_depth;
	float m_x;
	float m_z;
};

#endif