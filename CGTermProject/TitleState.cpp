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
        glBindTexture(GL_TEXTURE_2D, textures->getTextureIndices()[ TextureTypes::TITLE ] ); // Bind the title screen texture
    
        // Draw a textured quad
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-10, -10, 20);
        glTexCoord2f(0, 1.0); glVertex3f(-10, 10, 20);
        glTexCoord2f(1.0, 1.0); glVertex3f(10, 10, 20);
        glTexCoord2f(1.0, 0); glVertex3f(10, -10, 20);
        glEnd();
    
        glDisable(GL_TEXTURE_2D);
        
    
        glRasterPos3f( -25.0, -18.0, 0.0 );
        glColor3f( 1.0, 1.0, 1.0 );
    
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