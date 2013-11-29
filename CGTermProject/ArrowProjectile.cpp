#include "ArrowProjectile.h"

#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static const float SPEED = 0.8;
static const float RADIUS = .2;
const static uint SLICES_AND_STACKS = 10;
const static float HALF_LENGTH = 2;

static const float PI = 3.141592653589793238462643383279502884; // TODO: get const from somewhere else

ArrowProjectile::ArrowProjectile( Vector center ) :
  BubbleProjectile( center, RADIUS, SPEED )
{
	m_type = ProjectileTypes::ARROW;
}


ArrowProjectile::~ArrowProjectile()
{
}

void ArrowProjectile::draw()
{
	// calculate angle of rotation
	const float angle = acos( m_velocity.unit().dotProduct( Vector( 0.0, 0.0, 1.0 ) ) ) * ( 180 / PI ) - 180;
	// calculate axis of rotation
	const Vector axis = Vector( 0.0, 0.0, 1.0 ).crossProduct( m_velocity );

	glPushMatrix();

	glColor3f( 0.0, 1.0, 0.0 );

	// move to position
	glTranslatef( getCenterX(), getCenterY(), getCenterZ() );

	// roatate to direction of pointing
	glRotatef( angle, axis.getX(), axis.getY(), axis.getZ() );

	// make "arrow tip"
	glutSolidSphere( .3, 10, 10 );

	// make front tip the "origin"
	glTranslatef( 0.0, 0.0, HALF_LENGTH );

	// draw arrow
	glutSolidCone( RADIUS, HALF_LENGTH, SLICES_AND_STACKS, SLICES_AND_STACKS );
	glRotatef( 180.0, 1.0, 0.0, 0.0 );
	glutSolidCone( RADIUS, HALF_LENGTH, SLICES_AND_STACKS, SLICES_AND_STACKS );

	glPopMatrix();
}