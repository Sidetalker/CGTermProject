#include "Crosshair.h"
#include <iostream>
#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static const float SCALE = .25;
static const float PI = 3.141592653589793238462643383279502884; // TODO: get const from somewhere else

Crosshair::Crosshair() :
  GameObject( 0, 0, 0 )
{
	setCenterX( 0 );
}

Crosshair::~Crosshair()
{
}

void Crosshair::draw()
{
	glDisable( GL_LIGHTING );

	glColor3f (1,0,0);

	glLineWidth( 2 );

	// draw cross
	glBegin( GL_LINES );
		glVertex3f( m_centerX,
			        m_centerY + ( 1 * SCALE ), 
					m_centerZ );
		glVertex3f( m_centerX,
			        m_centerY + ( -1 * SCALE ), 
					m_centerZ );
		glVertex3f( m_centerX + ( -1 * SCALE ),
			        m_centerY, 
					m_centerZ );
		glVertex3f( m_centerX + ( 1 * SCALE),
			        m_centerY, 
					m_centerZ );
	glEnd();

	// draw circle
	glBegin( GL_LINE_STRIP );
		for ( float i = 0; i < 2 * PI; i += PI / 6 )
		{
			glVertex3f( m_centerX + cos( i ) * SCALE, m_centerY + sin( i ) * SCALE, m_centerZ );
		}
	glEnd();

	glEnable( GL_LIGHTING );
}