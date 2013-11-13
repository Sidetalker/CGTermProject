#include "GameObject.h"

GameObject::GameObject( Vector center ) :
  m_center( center )
{
}


GameObject::~GameObject()
{
}

void GameObject::setCenterX( float centerX )
{
	m_center = Vector( centerX, getCenterY(), getCenterZ() );
}

void GameObject::setCenterY( float centerY )
{
	m_center = Vector( getCenterX(), centerY, getCenterZ() );
}

void GameObject::setCenterZ( float centerZ )
{
	m_center = Vector( getCenterX(), getCenterY(), centerZ );
}
