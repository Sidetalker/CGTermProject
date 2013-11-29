#ifndef PROJECTILE_DEFS_H
#define PROJECTILE_DEFS_H

// contains the projectile types
struct ProjectileTypes
{
	enum id
	{
		CANNONBALL,
		SPREAD,
		ARROW,
		RAY,
		
		NUM_PROJECTILES,
		BUBBLE,
		INVALID_PROJECTILE
	};
};

#endif