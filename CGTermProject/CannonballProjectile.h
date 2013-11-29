#ifndef CANNONBALL_PROJECTILE_H
#define CANNONBALL_PROJECTILE_H

#include "BubbleProjectile.h"

class CannonballProjectile : public BubbleProjectile
{
public:
	CannonballProjectile( Vector center );
	virtual ~CannonballProjectile();
};

#endif