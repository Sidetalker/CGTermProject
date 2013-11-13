#ifndef BUBBLE_PROJECTILE_H
#define BUBBLE_PROJECTILE_H

#include "BaseProjectile.h"

class BubbleProjectile : public BaseProjectile
{
public:
	BubbleProjectile( Vector center );
	virtual ~BubbleProjectile();

	virtual void draw();
	virtual void checkCollisions( BaseTarget* targets[], uint numTargets );
};

#endif