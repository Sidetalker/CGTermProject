#include "RayProjectile.h"


RayProjectile::RayProjectile( float centerX, float centerY, float centerZ ) :
  BaseProjectile( centerX, centerY, centerZ )
{
}


RayProjectile::~RayProjectile()
{
}

void RayProjectile::draw()
{
	// do nothing!
	// ray projectile is "too fast to be visible"
}

void RayProjectile::checkCollisions(  )
{

}
