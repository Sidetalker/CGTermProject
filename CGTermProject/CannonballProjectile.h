#ifndef CANNONBALL_PROJECTILE
#define CANNONBALL_PROJECTILE

#include "BubbleProjectile.h"

class CannonballProjectile : public BubbleProjectile
{
public:
	CannonballProjectile( Vector center );
	virtual ~CannonballProjectile();
};

#endif