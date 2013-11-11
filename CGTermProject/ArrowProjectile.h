#ifndef ARROW_PROJECTILE_H
#define ARROW_PROJECTILE_H

#include "BaseProjectile.h"

class ArrowProjectile : public BaseProjectile
{
public:
	ArrowProjectile( float centerX, float centerY, float centerZ );
	~ArrowProjectile();

	virtual void draw();

private:

};

#endif