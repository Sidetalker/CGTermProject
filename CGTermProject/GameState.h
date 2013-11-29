#ifndef H_GAME_STATE
#define H_GAME_STATE

#include <vector>
#include <list>

#include "BaseProjectile.h"
#include "BaseState.h"
#include "Target.h"
#include "Crosshair.h"

#define TARGET_COUNT 5 // TODO: remove dependency

class GameState : public BaseState
{
public:
	GameState();
	virtual ~GameState();

	virtual void update();
	virtual void keyInput( unsigned char key, int x, int y );
	virtual void mouseAction( int button, int state, int x, int y );
	virtual void mouseMotion( int x, int y );

private:
	ProjectileTypes::id m_curProjectile;

	unsigned int m_numTargets;

	int m_score;

	float clickX;
	float clickY;
	float clickZ;

	Crosshair* m_pCrosshair;

	// Global array of targets
	BaseTarget* arrayTargets[ TARGET_COUNT ];

	std::list< BaseProjectile* > m_activeProjectiles;

	void updateActiveProjectiles();
	void drawActiveProjectiles();
	
	void setup();
	void drawHUD();

	// TODO: remove later...
	void testDrawShot();
};

#endif