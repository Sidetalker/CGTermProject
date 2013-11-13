#include "BubbleProjectile.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const static float RADIUS = 1;
const static uint SLICES_AND_STACKS = 10;

BubbleProjectile::BubbleProjectile( Vector center ) :
  BaseProjectile( center )
{
	m_type = Projectiles::BUBBLE;
}


BubbleProjectile::~BubbleProjectile()
{
}

// simply draws the object in the scene
void BubbleProjectile::draw()
{
	glPushMatrix();

	glTranslatef( getCenterX(), getCenterY(), getCenterZ() );
	glutSolidSphere( RADIUS, SLICES_AND_STACKS, SLICES_AND_STACKS );

	glPopMatrix();
}

void BubbleProjectile::checkCollisions( BaseTarget* targets[], uint numTargets )
{
	glColor3f( 0.0, 0.0, 1.0 );

	// TODO: this is a naiive check, improve on it
	for ( uint i = 0; i < numTargets; ++i )
	{
		// TODO: make target z value positive
		if ( ( m_center - Vector( targets[ i ]->getCenterX(), targets[ i ]->getCenterY(), -targets[ i ]->getCenterZ() ) ).magnitude() < 1 )
		{
			targets[ i ]->setIsHit( true );
		}
	}
}