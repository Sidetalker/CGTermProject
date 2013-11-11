#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject
{
public:
	GameObject( float centerX, float centerY, float centerZ );
	~GameObject();

	virtual void draw() = 0;

	float getCenterX() { return m_centerX; } const
	float getCenterY() { return m_centerY; } const
	float getCenterZ() { return m_centerZ; } const

	void setCenterX( float centerX ) { m_centerX = centerX; }
	void setCenterY( float centerY ) { m_centerY = centerY; }
	void setCenterZ( float centerZ ) { m_centerZ = centerZ; }

protected:
	float m_centerX;
	float m_centerY;
	float m_centerZ;
};

#endif