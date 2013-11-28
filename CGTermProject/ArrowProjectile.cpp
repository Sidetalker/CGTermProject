#include "ArrowProjectile.h"

static const float SPEED = 1.0;

ArrowProjectile::ArrowProjectile( Vector center ) :
  BaseProjectile( center, SPEED )
{
	m_type = ProjectileTypes::ARROW;
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