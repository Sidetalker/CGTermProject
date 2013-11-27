#ifndef H_TITLE_STATE
#define H_TITLE_STATE

#include "BaseState.h"

class TitleState : public BaseState
{
public:
	TitleState();
	virtual ~TitleState();

	virtual void update();
	virtual void keyInput( unsigned char key, int x, int y );
	virtual void mouseAction( int button, int state, int x, int y );
	virtual void mouseMotion( int x, int y );

private:	
	void setup();
};

#endif