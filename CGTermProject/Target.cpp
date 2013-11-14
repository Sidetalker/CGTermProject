#include "Target.h"

#include "Globals.h"
#include "Textures.h"
#include "TextureDefs.h"

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

	m_type = TargetTypes::BULLSEYE;
}

// Target constructor
Target::Target( Vector center, float r, unsigned char colorR,
				   unsigned char colorG, unsigned char colorB ) :
  BaseTarget( center, Vector(1, 0, 0), 0 )
, m_radius( r )
{
    color[ 0 ] = colorR;
    color[ 1 ] = colorG;
    color[ 2 ] = colorB;

	m_nearPlane = Plane( m_center +  Vector( 0.0, 0.0, HALF_THICKNESS ), Vector( 0.0, 0.0, 1.0 ) );
	m_farPlane = Plane( m_center - Vector( 0.0, 0.0, HALF_THICKNESS ), Vector( 0.0, 0.0, -1.0 ) );

	m_type = TargetTypes::BULLSEYE;
}

// Target constructor
Target::Target( Vector center, float r, Vector rotAxis, float rotAngle, unsigned char colorR,
				   unsigned char colorG, unsigned char colorB ) :
  BaseTarget( center, rotAxis, rotAngle )
, m_radius( r )
{
    color[ 0 ] = colorR;
    color[ 1 ] = colorG;
    color[ 2 ] = colorB;

	Vector newOffset = Vector( 0.0, 0.0, HALF_THICKNESS ).getRotatedVector( rotAxis, rotAngle );

	m_nearPlane = Plane( m_center +  newOffset, newOffset.unit() );
	m_farPlane = Plane( m_center - newOffset, newOffset.unit() );

	m_type = TargetTypes::BULLSEYE;
}

Target::~Target()
{
}

// Function to draw target TODO: figure out how to draw other side while still culling
void Target::draw()
{
    if ( m_radius > 0.0 ) // If target exists.
    {
        glPushMatrix();

        glTranslatef( getCenterX(), getCenterY(), getCenterZ() ); // move to proper position
        glColor3ubv( color ); // color

		glRotatef( m_rotAngle, m_rotAxis.getX(), m_rotAxis.getY(), m_rotAxis.getZ() ); // rotate target as specified

        GLUquadricObj *p = gluNewQuadric(); // Create a quadratic for the cylinder
        gluQuadricDrawStyle( p, GLU_FILL ); // Draw the quadratic as a fill

		glTranslatef( 0.0, 0.0, HALF_THICKNESS ); // move whole target forward one half thickness

		GLUquadric* quadDisk = gluNewQuadric(); // New quadratic for the face disk
        glEnable(GL_TEXTURE_2D); // Enable 2D textures
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Set texture environment parameters
		glBindTexture(GL_TEXTURE_2D, textures->getTextureIndices()[ TextureTypes::BULLSEYE ] ); // Bind the round target texture
		gluQuadricTexture(quadDisk, GL_TRUE); // Enable textures for the quadratic
		gluDisk(quadDisk, 0, m_radius, 100, 100 ); // Draw the target face
		gluQuadricTexture(quadDisk, GL_FALSE); // Disable textures for the quadtratic
        glDisable(GL_TEXTURE_2D); // Disable 2D textures

		glTranslatef( 0.0, 0.0, -HALF_THICKNESS * 2 ); // move whole target back one whole thickness

        gluCylinder( p, m_radius, m_radius, HALF_THICKNESS * 2, 30, 2 ); // Draw the target with bottom at z = 0
        gluDisk( p, 0, m_radius, 100, 100 ); // Draw the target back face at z = 0

        glPopMatrix();
        
		// normals FOR TESTING PURPOSES ONLY
		Vector temp = (m_nearPlane.getPoint() - m_center) * 5;

		Vector temp2 = (m_farPlane.getPoint() - m_center) * 5;

		glColor3f( 0,1,0 );
		glBegin( GL_LINE_STRIP );

		glVertex3f( m_center.getX() + temp.getX(), m_center.getY() + temp.getY(), m_center.getZ() + temp.getZ() );
		glVertex3f( m_center.getX(), m_center.getY(), m_center.getZ() );

		glVertex3f( m_center.getX() + temp2.getX(), m_center.getY() + temp2.getY(), m_center.getZ() + temp2.getZ() );
		glVertex3f( m_center.getX(), m_center.getY(), m_center.getZ() );

		glEnd();
		//}
		// end of normals FOR TESTING PURPOSES ONLY
    }
}
