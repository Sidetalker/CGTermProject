#include "CannonballProjectile.h"

static const float SPEED = 0.6;
static const float RADIUS = 1.0;

CannonballProjectile::CannonballProjectile( Vector center ) :
  BubbleProjectile( center, RADIUS, SPEED )
{
	m_type = ProjectileTypes::CANNONBALL;
}

CannonballProjectile::~CannonballProjectile()
{
}