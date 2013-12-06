#ifndef H_STATE_HANDLER
#define H_STATE_HANDLER

#include "BaseState.h"
#include "GameState.h"
#include "StateDefs.h"

class StateHandler
{
public:
	StateHandler();
	~StateHandler();

	// changes state and frees memory
	void changeState( StateTypes::id newState );
	StateTypes::id getStateId();

private:
	// current state object pointer
	static BaseState* m_pCurState;

	// GLUT callback functions
	static void animate( int value );
	static void update();
	static void resize( int width, int height );
	static void keyInput( unsigned char key, int x, int y );
	static void mouseAction( int button, int state, int x, int y );
	static void mouseMotion( int x, int y );
};

#endif