#ifndef RAY_PROJECTILE_H
#define RAY_PROJECTILE_H

#include "BaseProjectile.h"

class RayProjectile : BaseProjectile
{
public:
	RayProjectile( float centerX, float centerY, float centerZ );
	~RayProjectile();

	virtual void draw();
	virtual void checkCollisions( BaseTarget* targets[], uint numTargets );

private:

};

#endif