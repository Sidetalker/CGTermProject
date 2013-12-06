#ifndef H_GAME_STATE
#define H_GAME_STATE

#include <vector>
#include <list>

#include "BaseProjectile.h"
#include "BaseState.h"
#include "Target.h"
#include "Crosshair.h"
#include "FileReader.h"

#define TARGET_COUNT 3

// GameState class is the main game class
class GameState : public BaseState
{
public:

	// different stages within the game
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

	// virtual functions called by the StateHandler GLUT callbacks
	virtual void update();
	virtual void keyInput( unsigned char key, int x, int y );
	virtual void mouseAction( int button, int state, int x, int y );
	virtual void mouseMotion( int x, int y );

private:
	// current game stage update loop
	void ( GameState::*m_stageUpdate )();

	// game stage functions
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

	// current game stage
	Stages::id m_curStage;

private:
	// projectile being used
	ProjectileTypes::id m_curProjectile;

	// number of targets in game
	unsigned int m_numTargets;

	// Score (consists of uint score and initials)
	Score m_playerScore;

	// game round number
	uint m_round;

	// number of targets currently active
	uint m_numActiveTargets;

	// position that was clicked
	float clickX;
	float clickY;
	float clickZ;

	// game timer
	float m_timer;

	// crosshair object
	Crosshair* m_pCrosshair;

	// Global array of targets
	BaseTarget* arrayTargets[ TARGET_COUNT ];

	// high scores read from file
	std::vector < Score > m_highScores;

	// all targets read from file
	std::vector < BaseTarget* > m_targetData;

	// currently active projectiles in scene
	std::list< BaseProjectile* > m_activeProjectiles;

	// floor animation
	float m_floorAlpha;
	bool m_bFloorAlphaIncreasing;
	
	// called in constructor
	void setup();

	// draw objects
	void drawHUD();
	void drawHighScores();
	void drawFloor();
	void drawWalls();

	// update objects
	void updateActiveProjectiles();
	void drawActiveProjectiles();
	void updateFloor();
};

#endif