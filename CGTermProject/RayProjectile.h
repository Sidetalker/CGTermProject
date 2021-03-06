#ifndef RAY_PROJECTILE_H
#define RAY_PROJECTILE_H

#include "BaseProjectile.h"

// invisible ray
class RayProjectile : public BaseProjectile
{
public:
	RayProjectile( Vector center );
	virtual ~RayProjectile();

	virtual void draw();
	virtual void checkCollisions( BaseTarget* targets[], uint numTargets );

private:

};

#endif