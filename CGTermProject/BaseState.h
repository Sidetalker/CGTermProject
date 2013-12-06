#ifndef H_BASE_STATE
#define H_BASE_STATE

class BaseState
{
public:
	BaseState();
	virtual ~BaseState();

	// virtual functions called by StateHandler
	virtual void update() = 0;
	virtual void keyInput( unsigned char key, int x, int y ) = 0;
	virtual void mouseAction( int button, int state, int x, int y ) = 0;
	virtual void mouseMotion( int x, int y ) = 0;
};

#endif