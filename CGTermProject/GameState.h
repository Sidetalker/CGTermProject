#ifndef H_GAME_STATE
#define H_GAME_STATE

#include <vector>
#include <list>

#include "BaseProjectile.h"
#include "BaseState.h"
#include "Target.h"
#include "Crosshair.h"
#include "FileReader.h"

#define TARGET_COUNT 3 // TODO: remove dependency

class GameState : public BaseState
{
public:

	struct Stages
	{
		enum id
		{
			INTRO,
			ROUND,
			END_ROUND,
			EXIT,
			ENTER_INITIALS,
			HIGH_SCORES,
			OUTRO,

			INVALID_STAGE
		};
	};

	GameState();
	virtual ~GameState();

	virtual void update();
	virtual void keyInput( unsigned char key, int x, int y );
	virtual void mouseAction( int button, int state, int x, int y );
	virtual void mouseMotion( int x, int y );

private:
	void ( GameState::*m_stageUpdate )();

	void setIntro();
	void updateIntro();
	void setRound();
	void updateRound();
	void setEndRound();
	void updateEndRound();
	void setExit();
	void updateExit();
	void setEnterInitials();
	void updateEnterInitials();
	void setHighScores();
	void updateHighScores();
	void setOutro();
	void updateOutro();

private:
	ProjectileTypes::id m_curProjectile;

	Stages::id m_curStage;

	unsigned int m_numTargets;

	Score m_playerScore;

	uint m_round;

	uint m_numActiveTargets;

	float clickX;
	float clickY;
	float clickZ;

	float m_timer;

	float m_floorAlpha;
	bool m_bFloorAlphaIncreasing;

	Crosshair* m_pCrosshair;

	// Global array of targets
	BaseTarget* arrayTargets[ TARGET_COUNT ];

	std::vector < Score > m_highScores;

	std::vector < BaseTarget* > m_targetData;

	std::list< BaseProjectile* > m_activeProjectiles;

	void updateActiveProjectiles();
	void drawActiveProjectiles();
	void updateFloor();
	
	void setup();
	void drawHUD();
	void drawHighScores();
	void drawFloor();
	void drawWalls();

	// TODO: remove later...
	void testDrawShot();
};

#endif