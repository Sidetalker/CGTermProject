#include "BaseProjectile.h"


BaseProjectile::BaseProjectile( Vector center ) :
  GameObject( center )
, m_type( Projectiles::INVALID_PROJECTILE )
, m_velocity( 0, 0, 0 )
{
}


BaseProjectile::~BaseProjectile()
{
}