#include "BaseProjectile.h"


BaseProjectile::BaseProjectile( float centerX, float centerY, float centerZ ) :
  GameObject( centerX, centerY, centerZ )
, m_velocity( 0, 0, 0 )
{
}


BaseProjectile::~BaseProjectile()
{
}