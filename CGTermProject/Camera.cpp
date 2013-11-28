#include "Camera.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

Camera::Camera(
	float posX, 
	float posY, 
	float posZ, 
	float frustumWidth,
	float frustumHeight,
	float frustumNear,
	float frustumFar,
	float windowWidth,
	float windowHeight) :
  m_posX( posX )
, m_posY( posY )
, m_posZ( posZ )
, m_centerX( posX )
, m_centerY( posY )
, m_centerZ( 0 ) // change later
, m_frustumHeight( frustumHeight )
, m_frustumWidth( frustumWidth )
, m_frustumNear( frustumNear )
, m_frustumFar( frustumFar )
, m_frustumHalfWidth( frustumWidth / 2 )
, m_frustumHalfHeight( frustumHeight / 2 )
, m_windowHeight( windowHeight )
, m_windowWidth( windowWidth )
, m_windowHalfHeight( windowHeight / 2 )
, m_windowHalfWidth( windowWidth / 2 )
{
}


Camera::~Camera()
{
}

// referenced http://iphonedevelopment.blogspot.com/2010/02/drawing-hud-display-in-opengl-es.html
// changes to orthographic projection (for HUD elements)
void Camera::switchToOrtho()
{
	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	// change to actual window coordinate projection
	glOrtho( 0, m_windowWidth, 0, m_windowHeight, 0, 1 );  

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

// referenced http://iphonedevelopment.blogspot.com/2010/02/drawing-hud-display-in-opengl-es.html
// returns from orthographic projection to standard perspective projection
void Camera::returnToFrustum()
{
	glEnable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
}
