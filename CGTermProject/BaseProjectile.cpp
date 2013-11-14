#include "BaseProjectile.h"


BaseProjectile::BaseProjectile( Vector center ) :
  GameObject( center )
, m_type( ProjectileTypes::INVALID_PROJECTILE )
, m_velocity( 0, 0, 0 )
{
}


BaseProjectile::~BaseProjectile()
{
}

// moves projectile to next position based on velocity
void BaseProjectile::applyVelocity()
{
	m_center = m_center + m_velocity;
}