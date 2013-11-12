#include "RayProjectile.h"

#include "Plane.h"


RayProjectile::RayProjectile( float centerX, float centerY, float centerZ ) :
  BaseProjectile( centerX, centerY, centerZ )
{
	m_type = Projectiles::RAY;
}


RayProjectile::~RayProjectile()
{
}

void RayProjectile::draw()
{
	// do nothing!
	// ray projectile is "too fast to be visible"
}

void RayProjectile::checkCollisions( BaseTarget* targets[], uint numTargets )
{
	// iterate through all current targets
	for ( uint i = 0; i < numTargets; ++i )
	{
		switch ( targets[ i ]->getType() )
		{
			case Targets::BULLSEYE:
			{
				break;
			}
			default:
			{
				// invalid target type
				break;
			}
		}
	}
}
