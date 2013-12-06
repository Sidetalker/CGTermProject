#include "Crosshair.h"
#include <iostream>
#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static const float SCALE = .25;
static const float PI = 3.141592653589793238462643383279502884;

Crosshair::Crosshair() :
  GameObject( Vector() )
{
	setCenterX( 0 );
}

Crosshair::~Crosshair()
{
}

// draws crosshair to mouse position
void Crosshair::draw()
{
	glDisable( GL_LIGHTING );

	glColor3f (1,0,0);

	glLineWidth( 2 );

	const float centerX = getCenterX();
	const float centerY = getCenterY();
	const float centerZ = getCenterZ();

	// draw cross
	glBegin( GL_LINES );
		glVertex3f( centerX,
			        centerY + ( 1 * SCALE ), 
					centerZ );
		glVertex3f( centerX,
			        centerY + ( -1 * SCALE ), 
					centerZ );
		glVertex3f( centerX + ( -1 * SCALE ),
			        centerY, 
					centerZ );
		glVertex3f( centerX + ( 1 * SCALE),
			        centerY, 
					centerZ );
	glEnd();

	// draw circle
	glBegin( GL_LINE_STRIP );
		for ( float i = 0; i < 2 * PI; i += PI / 6 )
		{
			glVertex3f( centerX + cos( i ) * SCALE, centerY + sin( i ) * SCALE, centerZ );
		}
	glEnd();

	glEnable( GL_LIGHTING );
}