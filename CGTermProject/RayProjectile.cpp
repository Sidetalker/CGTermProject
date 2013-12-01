#include "RayProjectile.h"

#include <cmath>

#include "Target.h"
#include "Plane.h"

static const float SPEED = 1000.0;

RayProjectile::RayProjectile( Vector center ) :
  BaseProjectile( center, SPEED )
{
	m_type = ProjectileTypes::RAY;
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
		// prevent double hit for one projectile
		if ( m_bHitObject == true )
		{
			break;
		}

		// prevent double hit target
		if ( targets[ i ]->getStatus() == TargetStatus::ACTIVE )
		{
			switch ( targets[ i ]->getType() )
			{
				case TargetTypes::BULLSEYE: // TODO: can be cleaned up?
				{
					Target* t = ( Target* ) targets[ i ];

					// check intersection with front circle face
					Vector intersectionNear = t->getNearPlane().lineIntersect( Line( m_center, m_velocity ) );

					if ( ( intersectionNear - t->getNearPlane().getPoint() ).squareMagnitude() <= pow( t->getRadius(), 2 ) )
					{
						targets[ i ]->setStatus( TargetStatus::HIT );
						m_bHitObject = true;
						break;
					}

					// check intersection with back circle face
					Vector intersectionFar = t->getFarPlane().lineIntersect( Line( m_center, m_velocity ) );

					if ( ( intersectionFar - t->getFarPlane().getPoint() ).squareMagnitude() <= pow( t->getRadius(), 2 ) )
					{
						targets[ i ]->setStatus( TargetStatus::HIT );
						m_bHitObject = true;
					}

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
}
