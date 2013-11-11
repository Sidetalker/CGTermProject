#ifndef BASE_PROJECTILE_H
#define BASE_PROJECTILE_H

#include <vector>

#include "GameObject.h"
#include "Vector.h"
#include "Octree.h"

class BaseProjectile : public GameObject
{
public:
	BaseProjectile( float centerX, float centerY, float centerZ );
	~BaseProjectile();

	void setVelocity( Vector velocity ) { m_velocity = velocity; }

	virtual void checkCollisions( Octree* targets ) = 0; // TODO: takes octree pointer to check collisions?

protected:
	Vector m_velocity;

};

#endif