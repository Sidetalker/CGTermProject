#ifndef ARROW_PROJECTILE_H
#define ARROW_PROJECTILE_H

#include "BaseProjectile.h"

class ArrowProjectile : public BaseProjectile
{
public:
	ArrowProjectile( Vector center );
	virtual ~ArrowProjectile();

	virtual void draw();
	virtual void checkCollisions( BaseTarget* targets[], uint numTargets );

private:

};

#endif