#include "Target.h"
#include "Textures.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static const float THICKNESS = 0.5;

// Target default constructor
Target::Target() :
  BaseTarget( 0.0, 0.0, 0.0 )
, m_radius( 0.0 ) // Indicates no target exists in the position.
{
    color[ 0 ] = 0;
    color[ 1 ] = 0;
    color[ 2 ] = 0;
}

// Target constructor
Target::Target(float x, float y, float z, float r, unsigned char colorR,
				   unsigned char colorG, unsigned char colorB) :
  BaseTarget( x, y, z )
, m_radius( r )
, m_nearPlane() // TODO: make actual plane
, m_farPlane()  // TODO: make actual plane
{
    color[ 0 ] = colorR;
    color[ 1 ] = colorG;
    color[ 2 ] = colorB;
}

// Function to draw target
void Target::draw()
{
    if ( m_radius > 0.0 ) // If target exists.
    {
        glPushMatrix();
        glTranslatef(m_centerX, m_centerY, -m_centerZ);
        //glRotatef(20, 0, 1, 0);
        glColor3ubv(color);
        GLUquadricObj *p = gluNewQuadric(); // Create a quadratic for the cylinder
        gluQuadricDrawStyle( p, GLU_FILL ); // Draw the quadratic as a wireframe
        gluCylinder(p, m_radius, m_radius, THICKNESS, 30, 2); // Draw the target
        glTranslatef(0, 0, THICKNESS);
        GLUquadric* quadDisk = gluNewQuadric(); // New quadratic for the face disk
        glEnable(GL_TEXTURE_2D); // Enable 2D textures
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Set texture environment parameters
		glBindTexture(GL_TEXTURE_2D, texture[0]); // Bind the round target texture
		gluQuadricTexture(quadDisk, GL_TRUE); // Enable textures for the quadratic
		gluDisk(quadDisk, 0, m_radius, 100, 100 ); // Draw the target face
		gluQuadricTexture(quadDisk, GL_FALSE); // Disable textures for the quadtratic
        glDisable(GL_TEXTURE_2D); // Disable 2D textures
        glPopMatrix();
    }
}
