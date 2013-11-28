#ifndef BUBBLE_PROJECTILE_H
#define BUBBLE_PROJECTILE_H

#include "BaseProjectile.h"

class BubbleProjectile : public BaseProjectile
{
public:
	BubbleProjectile( Vector center, float radius, float speed );
	virtual ~BubbleProjectile();

	virtual void draw();
	virtual void checkCollisions( BaseTarget* targets[], uint numTargets );

private:
	float m_radius;
};

#endif