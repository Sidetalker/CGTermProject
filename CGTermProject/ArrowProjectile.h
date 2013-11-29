#ifndef ARROW_PROJECTILE_H
#define ARROW_PROJECTILE_H

#include "BubbleProjectile.h"

class ArrowProjectile : public BubbleProjectile
{
public:
	ArrowProjectile( Vector center );
	virtual ~ArrowProjectile();

	virtual void draw();

private:

};

#endif