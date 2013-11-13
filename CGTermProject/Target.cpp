#include "Target.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static const float HALF_THICKNESS = 0.5;

// Target default constructor
Target::Target() :
  BaseTarget( Vector() )
, m_radius( 0.0 ) // Indicates no target exists in the position.
{
    color[ 0 ] = 0;
    color[ 1 ] = 0;
    color[ 2 ] = 0;

	m_type = Targets::BULLSEYE;
}

// Target constructor
Target::Target( Vector center, float r, unsigned char colorR,
				   unsigned char colorG, unsigned char colorB ) :
  BaseTarget( center )
, m_radius( r )
, m_nearPlane() // TODO: make actual plane
, m_farPlane()  // TODO: make actual plane
{
    color[ 0 ] = colorR;
    color[ 1 ] = colorG;
    color[ 2 ] = colorB;

	m_nearPlane = Plane( m_center +  Vector( 0.0, 0.0, HALF_THICKNESS ), Vector( 0.0, 0.0, 1.0 ) );
	m_farPlane = Plane( m_center - Vector( 0.0, 0.0, HALF_THICKNESS ), Vector( 0.0, 0.0, -1.0 ) );

	m_type = Targets::BULLSEYE;
}

// Function to draw target
void Target::draw()
{
    if ( m_radius > 0.0 ) // If target exists.
    {
        glPushMatrix();
        glTranslatef( getCenterX(), getCenterY(), getCenterZ() );
        glColor3ubv( color );
        GLUquadricObj *p = gluNewQuadric(); // Create a quadratic for the cylinder
        gluQuadricDrawStyle( p, GLU_FILL ); // Draw the quadratic as a wireframe
        gluCylinder( p, m_radius, m_radius, HALF_THICKNESS * 2, 30, 2 ); // Draw the target
        gluDisk( p, 0, m_radius, 100, 100 ); // Draw the target face
        glPopMatrix();
        
//        void gluCylinder(	GLUquadric*  	quad,
//                         GLdouble  	base,
//                         GLdouble  	top,
//                         GLdouble  	height,
//                         GLint  	slices,
//                         GLint  	stacks);

    }
}
