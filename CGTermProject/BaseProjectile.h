#ifndef BASE_PROJECTILE_H
#define BASE_PROJECTILE_H

#include "GameObject.h"
#include "BaseTarget.h"
#include "Vector.h"
#include "ProjectileDefs.h"

class BaseProjectile : public GameObject
{
public:
	BaseProjectile( Vector center, float speed );
	virtual ~BaseProjectile();

	void setVelocity( Vector velocity );

	virtual void checkCollisions( BaseTarget* targets[], uint numTargets ) = 0;

	void applyVelocity();

	bool hasHitObject() const { return m_bHitObject; }

protected:
	bool m_bHitObject;

	// type of projectile
	ProjectileTypes::id m_type;

	Vector m_velocity;

	Vector m_prevPosition;

private:
	float m_speed;
};

#endif