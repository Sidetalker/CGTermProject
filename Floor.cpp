#include "Floor.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static const float COLOR_ONE[ 3 ] =
{
	0.922,
	0.106,
	0.141
};

static const float COLOR_TWO[ 3 ] =
{
	0.996,
	0.882,
	0.239
};

Floor::Floor( float x, float z, float squareSize, float width, float depth ) :
  m_x( x )
, m_z( z )
, m_squareSize( squareSize )
, m_width( width )
, m_depth( depth )
{
}

Floor::~Floor(void)
{
}

void Floor::draw()
{
	bool color=true;
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glPushMatrix();
	glTranslatef( 0.0, -25.0, 0.0 );
	glRotatef( -90.0, 1.0, 0.0, 0.0 );
	for ( float z = m_z; z < m_z + m_depth; z += m_squareSize )
	{
		for ( float x = m_x; x < m_x + m_width; x += m_squareSize, color = !color )
		{
			if ( color )
			{
				glColor3f( COLOR_ONE[ 0 ], COLOR_ONE[ 1 ], COLOR_ONE[ 2 ] );
			}
			else
			{
				glColor3f( COLOR_TWO[ 0 ], COLOR_TWO[ 1 ], COLOR_TWO[ 2 ] );
			}
			glRectf( x, z, x + m_squareSize, z + m_squareSize );
		}
		color = !color;
	}
	glPopMatrix();
}