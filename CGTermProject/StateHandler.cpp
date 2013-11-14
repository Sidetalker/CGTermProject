#include "StateHandler.h"

#include "Globals.h"
#include "Game.h"
#include "GameState.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


BaseState* StateHandler::m_pCurState = 0;

StateHandler::StateHandler()
{
	// TODO: currently a default game state, maybe have an intro state
	// TODO: currently a hardcoded current state, change earlier
	m_pCurState = new GameState();

	glutKeyboardFunc( keyInput );
	glutDisplayFunc( update );
	glutReshapeFunc( resize );
	glutTimerFunc( 5, animate, 1 );
	glutMouseFunc( mouseAction );
	glutPassiveMotionFunc( mouseMotion );
	glutMotionFunc( mouseMotion ); // TODO: Should they be separate functions?
}


StateHandler::~StateHandler()
{
	delete m_pCurState;
}

void StateHandler::animate( int value )
{
	glutTimerFunc(5, animate, 1);
	glutPostRedisplay();
}

void StateHandler::update()
{
	m_pCurState->update();
}

void StateHandler::keyInput( unsigned char key, int x, int y )
{
	m_pCurState->keyInput( key, x, y );
}

void StateHandler::mouseAction( int button, int state, int x, int y )
{
	m_pCurState->mouseAction( button, state, x, y );
}

void StateHandler::resize( int width, int height )
{
	// set window size
	game->getCamera()->setWindowWidth( width );
	game->getCamera()->setWindowHeight( height );

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    
    glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();
	glFrustum(
		-game->getCamera()->getFrustumHalfWidth(),
		game->getCamera()->getFrustumHalfWidth(),
		-game->getCamera()->getFrustumHalfHeight(),
		game->getCamera()->getFrustumHalfHeight(),
		game->getCamera()->getFrustumNear(),
		game->getCamera()->getFrustumFar());

    glMatrixMode(GL_MODELVIEW);
}

void StateHandler::mouseMotion( int x, int y )
{
	m_pCurState->mouseMotion( x, y );
}