#ifndef H_GAME
#define H_GAME

#include "Camera.h"
#include "StateHandler.h"

// game class is current instance of play
class Game
{
public:
	Game();
	~Game();

	Camera* getCamera() const { return m_pCamera; }
	StateHandler* getStateHandler() const { return m_pStateHandler; }

private:
	Camera* m_pCamera;
	StateHandler* m_pStateHandler;
};

#endif