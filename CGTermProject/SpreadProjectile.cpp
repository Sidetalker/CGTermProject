#include "SpreadProjectile.h"

static const float SPEED = 5.0;
static const float RADIUS = 0.3;

SpreadProjectile::SpreadProjectile( Vector center ) :
  BubbleProjectile( center, RADIUS, SPEED )
{
}

SpreadProjectile::~SpreadProjectile()
{
}