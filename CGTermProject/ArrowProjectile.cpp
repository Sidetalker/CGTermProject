#include "ArrowProjectile.h"


ArrowProjectile::ArrowProjectile( float centerX, float centerY, float centerZ ) :
  BaseProjectile( centerX, centerY, centerZ )
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