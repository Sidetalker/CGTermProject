#include "TitleState.h"

#include "StateDefs.h"
#include "Globals.h"
#include "Game.h"
#include "Textures.h"
#include "TextureDefs.h"

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

	glLoadIdentity();
	gluLookAt(0, 0, 30, 0, 0, 0, 0.0, 1.0, 0.0);

	glPushMatrix();
        glEnable(GL_TEXTURE_2D);
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

        glBindTexture(GL_TEXTURE_2D, textures->getTextureIndices()[ TextureTypes::TITLE ] ); // Bind the title screen texture
    
        // Draw a textured quad
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-10, -10, 20);
        glTexCoord2f(0, 1.0); glVertex3f(-10, 10, 20);
        glTexCoord2f(1.0, 1.0); glVertex3f(10, 10, 20);
        glTexCoord2f(1.0, 0); glVertex3f(10, -10, 20);
        glEnd();
    
        glDisable(GL_TEXTURE_2D);
        
    
		glColor3f( 1.0, 1.0, 1.0 );

		glPushMatrix();

		game->getCamera()->switchToOrtho();
		glRasterPos2f( game->getCamera()->getWindowHalfWidth() - ( 11 * 9 ), game->getCamera()->getWindowHeight() * 0.25 );
		for ( char* c = "PRESS ANY KEY TO BEGIN"; *c != '\0'; c++ )
			glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *c );

		game->getCamera()->returnToPerspective();

		// pop to MVM
		glPopMatrix();

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