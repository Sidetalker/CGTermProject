#ifndef PROJECTILE_DEFS_H
#define PROJECTILE_DEFS_H

// contains the projectile types
struct ProjectileTypes
{
	enum id
	{
		CANNONBALL,
		BUBBLE,
		SPREAD,
		ARROW,
		RAY,
		
		NUM_PROJECTILES,
		INVALID_PROJECTILE
	};
};

#endif