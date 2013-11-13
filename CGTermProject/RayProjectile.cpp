#include "RayProjectile.h"

#include "Plane.h"


RayProjectile::RayProjectile( Vector center ) :
  BaseProjectile( center )
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
