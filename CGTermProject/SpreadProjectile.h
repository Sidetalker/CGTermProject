#ifndef SPREAD_PROJECTILE_H
#define SPREAD_PROJECTILE_H

#include "BubbleProjectile.h"

class SpreadProjectile : public BubbleProjectile
{
public:
	SpreadProjectile( Vector center );
	virtual ~SpreadProjectile();
};

#endif