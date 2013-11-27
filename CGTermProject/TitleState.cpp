#include "TitleState.h"

#include "StateDefs.h"
#include "Globals.h"
#include "Game.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

TitleState::TitleState()
{
	setup();
}

TitleState::~TitleState()
{
}

void TitleState::update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: make easier to use, maybe have a method of the camera
	// to automatically load the camera view.
	//{
	const float eyeX = game->getCamera()->getPosX();
	const float eyeY = game->getCamera()->getPosY();
	const float eyeZ = game->getCamera()->getPosZ();

	const float centerX = game->getCamera()->getCenterX();
	const float centerY = game->getCamera()->getCenterY();
	const float centerZ = game->getCamera()->getCenterZ();

	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);
	// } END OF TODO: make easier to use


	glPushMatrix();
		glColor3f( 1.0, 1.0, 1.0 );
		glRasterPos3f( -25.0, 0.0, 0.0 );

		// TODO: Make actual title screen
		for ( char* c = "PRESS ANY KEY TO BEGIN (THIS IS A PLACEHOLDER TITLE SCREEN)"; *c != '\0'; c++ ) glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, *c );    
    glPopMatrix();


	glFlush();
}

void TitleState::keyInput( unsigned char key, int x, int y )
{
	game->getStateHandler()->changeState( StateTypes::GAMESTATE );
}

void TitleState::mouseAction( int button, int state, int x, int y )
{
}

void TitleState::mouseMotion( int x, int y )
{
}

void TitleState::setup()
{
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
}