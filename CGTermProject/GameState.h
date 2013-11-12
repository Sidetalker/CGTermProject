#ifndef H_GAME_STATE
#define H_GAME_STATE

#include <vector>

#include "BaseProjectile.h"
#include "BaseState.h"
#include "Target.h"
#include "Crosshair.h"

#define TARGET_COUNT 5 // TODO: remove dependency

class GameState : public BaseState
{
public:
	GameState();
	~GameState();

	virtual void update();
	virtual void keyInput( unsigned char key, int x, int y );
	virtual void mouseAction( int button, int state, int x, int y );
	virtual void mouseMotion( int x, int y );

private:

	unsigned int m_numTargets;

	int m_score;

	float clickX;
	float clickY;
	float clickZ;

	Crosshair* m_pCrosshair;

	// Global array of targets
	BaseTarget* arrayTargets[ TARGET_COUNT ];

	std::vector< BaseProjectile* > m_activeProjectiles;


	void testDrawShot();
	void testDrawProjectile();

	void setup();
};

#endif