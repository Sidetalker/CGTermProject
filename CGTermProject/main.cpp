
#include "Globals.h"
#include "Game.h"
#include "Textures.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Main routine.
int main( int argc, char **argv )
{
	// create global game object
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 500, 500 );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "Shooting Range Simulator" );

	// set game and load textures
	game = new Game();
	textures = new Textures();

	// cursor invisible
	glutSetCursor( GLUT_CURSOR_NONE ); 

    glutMainLoop();

    return 0;
}