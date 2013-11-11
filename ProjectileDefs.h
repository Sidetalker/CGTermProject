#ifndef PROJECTILE_DEFS_H
#define PROJECTILE_DEFS_H

// contains the projectile types
struct Projectiles
{
	enum id
	{
		BUBBLE,
		RAY,
		
		NUM_PROJECTILES,
		INVALID_PROJECTILE
	};
};

#endif