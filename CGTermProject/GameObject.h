#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Typedefs.h"

#include "Vector.h"

class GameObject
{
public:
	GameObject( Vector center );
	~GameObject();

	virtual void draw() = 0;

	float getCenterX() const { return m_center.getX(); }
	float getCenterY() const { return m_center.getY(); }
	float getCenterZ() const { return m_center.getZ(); }

	void setCenterX( float centerX );
	void setCenterY( float centerY );
	void setCenterZ( float centerZ );

protected:
	Vector m_center;
};

#endif