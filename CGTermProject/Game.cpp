#include "Game.h"

Game::Game()
{
	m_pCamera = new Camera();
	m_pStateHandler = new StateHandler();
	int x = 5;
}


Game::~Game()
{
	delete m_pCamera;
	delete m_pStateHandler;
}
