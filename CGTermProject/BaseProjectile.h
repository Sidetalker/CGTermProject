#ifndef BASE_PROJECTILE_H
#define BASE_PROJECTILE_H

#include <vector>

#include "GameObject.h"
#include "BaseTarget.h"
#include "Vector.h"
#include "ProjectileDefs.h"

class BaseProjectile : public GameObject
{
public:
	BaseProjectile( Vector center );
	~BaseProjectile();

	void setVelocity( Vector velocity ) { m_velocity = velocity; }

	// TODO:
	// Purpose: flags all hit targets as hit
	virtual void checkCollisions( BaseTarget* targets[], uint numTargets ) = 0;

protected:

	// type of projectile
	Projectiles::id m_type;

	Vector m_velocity;

};

#endif