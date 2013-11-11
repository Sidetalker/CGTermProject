#include <cstdlib>
#include <cmath>

#include "Globals.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

void setup()
{
    
}

// Main routine.
int main(int argc, char **argv)
{
	// create global game object
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize( 500, 500 );
    glutInitWindowPosition(100, 100);
    glutCreateWindow ("Empty Scene");
    
	game = new Game();
    
    glutMainLoop();
    
    return 0;
}