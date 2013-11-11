#include "Camera.h"

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
