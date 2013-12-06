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
static const float POINT_VALUE = 20.0;

static const unsigned char COLOR[] =
{
	252,
	196,
	0
};

// Target default constructor
Target::Target() :
  BaseTarget( Vector() )
, m_radius( 0.0 ) // Indicates no target exists in the position.
{
    color[ 0 ] = 0;
    color[ 1 ] = 0;
    color[ 2 ] = 0;

	m_type = TargetTypes::BULLSEYE;
	m_speed = 0;
}

// Target constructor
Target::Target( Vector center, float r, float speed ) :
  BaseTarget( center, Vector(1, 0, 0), POINT_VALUE )
, m_radius( r )
{
    color[ 0 ] = COLOR[ 0 ];
    color[ 1 ] = COLOR[ 1 ];
    color[ 2 ] = COLOR[ 2 ];

	// set surface plane data
	m_nearPlane = Plane( m_center +  Vector( 0.0, 0.0, HALF_THICKNESS ), Vector( 0.0, 0.0, 1.0 ) );
	m_farPlane = Plane( m_center - Vector( 0.0, 0.0, HALF_THICKNESS ), Vector( 0.0, 0.0, -1.0 ) );

	m_type = TargetTypes::BULLSEYE;
	m_rotAngle = 0;
	m_speed = speed;
}

// Target constructor
Target::Target( Vector center, float speed, float r, Vector rotAxis, float rotSpeed ) :
  BaseTarget( center, rotAxis, POINT_VALUE )
, m_radius( r )
{
    color[ 0 ] = COLOR[ 0 ];
    color[ 1 ] = COLOR[ 1 ];
    color[ 2 ] = COLOR[ 2 ];

	Vector newOffset = Vector( 0.0, 0.0, HALF_THICKNESS ).getRotatedVector( rotAxis, 0.0 );

	// set surface plane data
	m_nearPlane = Plane( m_center +  newOffset, newOffset.unit() );
	m_farPlane = Plane( m_center - newOffset, newOffset.unit() );

	m_type = TargetTypes::BULLSEYE;
	m_rotAngle = 0;
	m_speed = speed;
	m_rotSpeed = rotSpeed;
}

Target::~Target()
{
}

// function to draw target
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
		gluDisk(quadDisk, 0, m_radius, 15, 15 ); // Draw the target face
		gluQuadricTexture(quadDisk, GL_FALSE); // Disable textures for the quadtratic
		glDisable(GL_TEXTURE_2D); // Disable 2D textures

		glTranslatef( 0.0, 0.0, -HALF_THICKNESS * 2 ); // move whole target back one whole thickness

		gluCylinder( p, m_radius, m_radius, HALF_THICKNESS * 2, 15, 2 ); // Draw the target with bottom at z = 0

		// reverse orientation for back face
		gluQuadricOrientation(p,  GLU_INSIDE);
		gluDisk( p, 0, m_radius, 100, 100 ); // Draw the target back face at z = 0

		glPopMatrix();
	}
}

// reset normals
void Target::postUpdate()
{
	Vector newOffset = Vector( 0.0, 0.0, HALF_THICKNESS ).getRotatedVector( m_rotAxis, m_rotAngle );

	m_nearPlane = Plane( m_center +  newOffset, newOffset.unit() );
	m_farPlane = Plane( m_center - newOffset, newOffset.unit() );
}
