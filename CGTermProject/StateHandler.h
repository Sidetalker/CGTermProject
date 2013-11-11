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

	void changeState( States::id ); // TODO: implement state function
	States::id getStateId();        // TODO: implement state getter

private:
	static BaseState* m_pCurState;

	static void animate( int value );
	static void update();
	static void resize( int width, int height );
	static void keyInput( unsigned char key, int x, int y );
	static void mouseAction( int button, int state, int x, int y );
	static void mouseMotion( int x, int y );
};

#endif