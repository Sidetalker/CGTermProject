#include "ArrowProjectile.h"


ArrowProjectile::ArrowProjectile( Vector center ) :
  BaseProjectile( center )
{
	m_type = Projectiles::ARROW;
}


ArrowProjectile::~ArrowProjectile()
{
}

void ArrowProjectile::draw()
{

}

void ArrowProjectile::checkCollisions( BaseTarget* targets[], uint numTargets )
{

}