#include "BubbleProjectile.h"

#include <cmath>

#include "Target.h"
#include "Plane.h"


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const static uint SLICES_AND_STACKS = 10;

BubbleProjectile::BubbleProjectile( Vector center, float radius, float speed ) :
  BaseProjectile( center, speed )
, m_radius( radius )
{
	m_type = ProjectileTypes::BUBBLE;
}


BubbleProjectile::~BubbleProjectile()
{
}

// simply draws the object in the scene
void BubbleProjectile::draw()
{
	glColor3f( 1.0, 0.0, 0.0 );

	glPushMatrix();

	glTranslatef( getCenterX(), getCenterY(), getCenterZ() );
	glutSolidSphere( m_radius, SLICES_AND_STACKS, SLICES_AND_STACKS );

	glPopMatrix();
}

void BubbleProjectile::checkCollisions( BaseTarget* targets[], uint numTargets )
{
	// TODO: this is a naiive check, improve on it
	for ( uint i = 0; i < numTargets; ++i )
	{
		switch ( targets[ i ]->getType() )
		{
			case TargetTypes::BULLSEYE: // TODO: can be cleaned up?
			{
				// cast as Target
				Target* t = ( Target* ) targets[ i ];

				// Vector from near plane to current center
				Vector nearPlaneToCurCenter( ( m_center ) - ( t->getNearPlane().getPoint() ) );

				// Vector from near plane to previous center
				Vector nearPlaneToPrevCenter( ( m_prevPosition ) - ( t->getNearPlane().getPoint() ) );

				// if opposite signs (crossed the near plane) 
				if ( ( nearPlaneToPrevCenter.dotProduct( t->getNearPlane().getNormal() ) < 0 != nearPlaneToCurCenter.dotProduct( t->getNearPlane().getNormal() ) < 0 ) )
				{
					// point on near plane of intersection
					Vector intersectionNear = t->getNearPlane().lineIntersect( Line( m_prevPosition, m_velocity ) );

					// if point of intersection is within bounds of circle target face
					if ( ( intersectionNear - t->getNearPlane().getPoint() ).squareMagnitude() <= pow( ( t->getRadius() + m_radius ), 2 ) )
					{
						t->setStatus( TargetStatus::HIT );
						m_bHitObject = true;
						break;
					}
				}

				// Vector from far plane to current center
				Vector farPlaneToCurCenter( ( m_center ) - ( t->getFarPlane().getPoint() ) );

				// Vector from far plane to previous center
				Vector farPlaneToPrevCenter( ( m_prevPosition ) - ( t->getFarPlane().getPoint() ) );

				// if opposite signs (crossed the far plane) 
				if ( ( farPlaneToPrevCenter.dotProduct( t->getFarPlane().getNormal() ) < 0 != farPlaneToCurCenter.dotProduct( t->getFarPlane().getNormal() ) < 0 ) )
				{
					// point on far plane of intersection
					Vector intersectionFar = t->getFarPlane().lineIntersect( Line( m_prevPosition, m_velocity ) );

					// if point of intersection is within bounds of circle target face
					if ( ( intersectionFar - t->getFarPlane().getPoint() ).squareMagnitude() <= pow( ( t->getRadius() + m_radius ), 2 ) )
					{
						t->setStatus( TargetStatus::HIT );
						m_bHitObject = true;
					}
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