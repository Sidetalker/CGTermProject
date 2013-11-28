#include "BaseProjectile.h"

const static float GRAVITY = .01;

BaseProjectile::BaseProjectile( Vector center ) :
  GameObject( center )
, m_type( ProjectileTypes::INVALID_PROJECTILE )
, m_velocity( 0, 0, 0 )
, m_prevPosition( center )
{
}


BaseProjectile::~BaseProjectile()
{
}

// moves projectile to next position based on velocity
void BaseProjectile::applyVelocity()
{
	m_prevPosition = m_center;

	// apply gravity to non-instaneous projectiles before moving
	if ( m_type != ProjectileTypes::RAY )
	{
		m_velocity = m_velocity - Vector( 0.0, GRAVITY, 0.0 );
	}

	m_center = m_center + m_velocity;
}