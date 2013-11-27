#include <cstdlib>
#include <cmath>

#include "Globals.h"
#include "Game.h"
#include "Textures.h"

#include "Plane.h"

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Main routine.
int main(int argc, char **argv)
{
	// TESTING TODO: remove
	//Plane test( Vector( 1, 2, 3 ), Vector( -3, 5, -1) );
	//Line testLine( Vector( 0, 0, 0 ), Vector( .1, .1, .1 ) );

	//std::cout << test.vectorIntersect( testLine ).getX() << " " <<
	//	test.vectorIntersect( testLine ).getY() << " " <<
	//	test.vectorIntersect( testLine ).getZ() << '\n';
	// TESTING

	// create global game object
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize( 500, 500 );
    glutInitWindowPosition(100, 100);
    glutCreateWindow ("Shooting Range Simulator");

	game = new Game();
	textures = new Textures();

    glutMainLoop();

    return 0;
}