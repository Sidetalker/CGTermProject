#ifndef H_GAME
#define H_GAME

#include "Camera.h"
#include "StateHandler.h"

class Game
{
public:
	Game();
	~Game();

	Camera* getCamera() const { return m_pCamera; }

private:
	Camera* m_pCamera;
	StateHandler* m_pStateHandler;
};

#endif