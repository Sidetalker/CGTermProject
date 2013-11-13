#include <cmath>
#include <iostream>
#include <fstream>

#include "GameState.h"
#include "Globals.h"
#include "Vector.h"
#include "Crosshair.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

// Texture count
#define TEXTURE_COUNT 1

// Texture indices
static unsigned int texture[TEXTURE_COUNT]; // Array of texture indices.

// outermost scene rotation FOR TESTING PURPOSES ONLY
float Yangle = 0;

// global step counter just FOR TESTING PURPOSES ONLY
float projectileStep = 0.0;

using namespace std;

GameState::GameState() :
  m_numTargets( TARGET_COUNT ) // TODO: remove TARGET_COUNT dependency
, m_score( 0 )
,  clickX( 0 )
, clickY( 0 )
, clickZ( 0 )
, m_pCrosshair( new Crosshair() )
, m_activeProjectiles()
//, m_objects( new Octree(0,0,0,0,0,0,0) )
{
	setup();
}

GameState::~GameState()
{
	// free crosshair memory
	delete m_pCrosshair;

	// free target memory
	for ( uint i = 0; i < m_numTargets; ++i )
	{
		delete arrayTargets[ i ];
	}

	// free projectile memory
	for ( uint i = 0; i < m_activeProjectiles.size(); ++i )
	{
		delete m_activeProjectiles[ i ];
	}
	
}

void GameState::update()
{
	const float eyeX = game->getCamera()->getPosX();
	const float eyeY = game->getCamera()->getPosY();
	const float eyeZ = game->getCamera()->getPosZ();

	const float centerX = game->getCamera()->getCenterX();
	const float centerY = game->getCamera()->getCenterY();
	const float centerZ = game->getCamera()->getCenterZ();


	int i = 0;
    float z = 100;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);

	glRotatef(Yangle, 0.0, 1.0, 0.0);
    
//    // Simple black floor
//    glColor3f(0.0, 0.0, 0.0);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    
//    for(float z=-150.0; z<100.0; z+=5.0)
//    {
//        glBegin(GL_TRIANGLE_STRIP);
//        for(float x=-100.0; x<100.0; x+=5.0)
//        {
//            glVertex3f(x, -10.0, z);
//            glVertex3f(x, -10.0, z+5.0);
//        }
//        glEnd();
//    }
    
    // Draw floor as a stack of triangle strips.
    for(z = 100.0; z > -100.0; z -= 5.0)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for(float x = -100.0; x < 100.0; x += 5.0)
        {
            if (i % 2) glColor4f(0.0, 0.0, 0.0, 1.0);
            else glColor4f(1.0, 1.0, 1.0, 1.0);
            glNormal3f(0.0, 1.0, 0.0);
            glVertex3f(x, 0.0, z - 5.0);
            glVertex3f(x, 0.0, z);
            i++;
        }
        glEnd();
        i++;
    }
    
    // Draw targets
    for (int i = 0; i < 1; i++)
    {
//		if ( !arrayTargets[ i ]->getIsHit() )
//			arrayTargets[i]->draw();
        glPushMatrix();
        glTranslatef( 0.0, 5.0, 20.0 );
        unsigned char color[ 3 ];
        color[0] = 255;
        color[1] = 255;
        color[2] = 255;
        glColor3ubv( color );
        GLUquadricObj *p = gluNewQuadric(); // Create a quadratic for the cylinder
        gluQuadricDrawStyle( p, GLU_FILL ); // Draw the quadratic as a wireframe
        gluCylinder( p, 2.0, 2.0, 0.5, 30, 2 ); // Draw the target
        
        GLUquadric* quadsphere = gluNewQuadric();
        glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		gluQuadricTexture(quadsphere, GL_TRUE);
		gluDisk( quadsphere, 0, 2.0, 100, 100 ); // Draw the target face
		gluQuadricTexture(quadsphere, GL_FALSE);
        glDisable(GL_TEXTURE_2D);
        
        
        
        glPopMatrix();
    }
    
	// Draw crosshair
	m_pCrosshair->draw();

	// FOR TESTING PURPOSES ONLY draw ray and projectile from eye through point clicked
	testDrawShot();
	testDrawProjectile();

    glFlush();
}

void GameState::mouseAction( int button, int state, int x, int y )
{
	const float eyeX = game->getCamera()->getPosX();
	const float eyeY = game->getCamera()->getPosY();
	const float eyeZ = game->getCamera()->getPosZ();

	const float frustumWidth = game->getCamera()->getFrustumWidth();
	const float frustumHalfWidth = game->getCamera()->getFrustumHalfWidth();
	const float frustumHeight = game->getCamera()->getFrustumHeight();
	const float frustumHalfHeight = game->getCamera()->getFrustumHalfHeight();
	const float frustumNear = game->getCamera()->getFrustumNear();

	const float windowWidth = game->getCamera()->getWindowWidth();
	const float windowHalfWidth = game->getCamera()->getWindowHalfWidth();
	const float windowHeight = game->getCamera()->getWindowHeight();
	const float windowHalfHeight = game->getCamera()->getWindowHalfHeight();

	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		clickX = (x + ((eyeX / (frustumHalfWidth))*(windowHalfWidth) - (windowHalfWidth)))/(windowWidth/frustumWidth);
		clickY = -(y - ((eyeY / (frustumHalfHeight))*(windowHalfHeight) + (windowHalfHeight)))/(windowHeight/frustumHeight);
		clickZ = eyeZ - frustumNear;
		projectileStep = 0.0; // FOR TESTING PURPOSES ONLY
	}

	glutPostRedisplay();
}

void GameState::mouseMotion( int x, int y )
{
	// TODO: Hack, fix later to be member of GameState

	const float eyeX = game->getCamera()->getPosX(); // TODO: make private function to map screen to scene
	const float eyeY = game->getCamera()->getPosY();
	const float eyeZ = game->getCamera()->getPosZ();

	const float frustumWidth = game->getCamera()->getFrustumWidth();
	const float frustumHalfWidth = game->getCamera()->getFrustumHalfWidth();
	const float frustumHeight = game->getCamera()->getFrustumHeight();
	const float frustumHalfHeight = game->getCamera()->getFrustumHalfHeight();
	const float frustumNear = game->getCamera()->getFrustumNear();

	const float windowWidth = game->getCamera()->getWindowWidth();
	const float windowHalfWidth = game->getCamera()->getWindowHalfWidth();
	const float windowHeight = game->getCamera()->getWindowHeight();
	const float windowHalfHeight = game->getCamera()->getWindowHalfHeight();

	float posX = (x + ((eyeX / (frustumHalfWidth))*(windowHalfWidth) - (windowHalfWidth)))/(windowWidth/frustumWidth);
	float posY = -(y - ((eyeY / (frustumHalfHeight))*(windowHalfHeight) + (windowHalfHeight)))/(windowHeight/frustumHeight);
	float posZ = eyeZ - frustumNear;
	
	m_pCrosshair->setCenterX( posX );
	m_pCrosshair->setCenterY( posY );
	m_pCrosshair->setCenterZ( posZ );

	glutPostRedisplay();
}

// Keyboard input processing routine.
void GameState::keyInput( unsigned char key, int x, int y )
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
		case 'y': // FOR TESTING PURPOSES ONLY
			Yangle += 5.0;
			if (Yangle > 360.0) Yangle -= 360.0;
			glutPostRedisplay();
			break;
		case 'Y': // FOR TESTING PURPOSES ONLY
			Yangle -= 5.0;
			if (Yangle < 0.0) Yangle += 360.0;
			glutPostRedisplay();
			break;
		case 'a': // display status of scene FOR TESTING PURPOSES ONLY
			for ( int i = 0; i < TARGET_COUNT; i++ )
			{
				std::cout << "Target " << i << ": " << arrayTargets[i]->getCenterX() << " " << arrayTargets[i]->getCenterY() << " " << arrayTargets[i]->getCenterZ() << std::endl;
			}
			break;
		case 'r': // reset projectile FOR TESTING PURPOSES ONLY
			break;
        default:
            break;
    }
}

// Struct of bitmap file.
struct BitMapFile
{
    int sizeX;
    int sizeY;
    unsigned char *data;
};

// Routine to read a bitmap file.
BitMapFile *getBMPData(string filename)
{
    BitMapFile *bmp = new BitMapFile;
    unsigned int size, offset, headerSize;
    
    // Read input file name.
    ifstream infile(filename.c_str(), ios::binary);
    
    if(!infile) {
        cout << "fail" << endl;
    }
    
    // Get the starting point of the image data.
    infile.seekg(10);
    infile.read((char *) &offset, 4);
    
    // Get the header size of the bitmap.
    infile.read((char *) &headerSize,4);
    
    // Get width and height values in the bitmap header.
    infile.seekg(18);
    infile.read( (char *) &bmp->sizeX, 4);
    infile.read( (char *) &bmp->sizeY, 4);
    
    // Allocate buffer for the image.
    size = bmp->sizeX * bmp->sizeY * 24;
    bmp->data = new unsigned char[size];
    
    // Read bitmap data.
    infile.seekg(offset);
    infile.read((char *) bmp->data , size);
    
    // Reverse color from bgr to rgb.
    int temp;
    for (int i = 0; i < size; i += 3)
    {
        temp = bmp->data[i];
        bmp->data[i] = bmp->data[i+2];
        bmp->data[i+2] = temp;
    }
    
    return bmp;
}

// Load external textures.
void loadExternalTextures()
{
    // Local storage for bmp image data.
    BitMapFile *image[TEXTURE_COUNT];
    
    // Load the textures.
    image[0] = getBMPData("/Users/sideslapd/Desktop/CGTermProject/CGTermProject/Textures/RoundTargetTexture.bmp");
    
    // Bind grass image to texture index[0].
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

// Initialization routine.
void GameState::setup()
{
	glutSetCursor( GLUT_CURSOR_NONE ); 

    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    // Lighting stuff from checkeredFloor.cpp
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    
    // Turn on OpenGL lighting.
    glEnable(GL_LIGHTING);
    
    // Load textures
    loadExternalTextures();
    
    // Light property vectors.
    float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
    float lightPos[] = {10.0, 30.0, 25.0, 1.0 };
    float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };
    
    // Light properties.
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    glEnable(GL_LIGHT0); // Enable particular light source.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
    
	// Material property vectors.
    float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
    float matShine[] = { 50.0 };
    
    // Material properties.
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
    
    // Enable color material mode.
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    // Flat shading to obtain the checkered pattern of the floor.
    glShadeModel(GL_FLAT);
    
    // Cull back faces.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // End lighting stuff
    
    // Initialize targets TODO: change to be explicit heap pointers
    arrayTargets[0] = new Target( 0.0, 5.0, 0.0, 2.0, 255, 0, 0 );
    arrayTargets[1] = new Target( 10.0, 5.0, -15.0, 2.0, 255, 0, 0 );
    arrayTargets[2] = new Target( -10.0, 5.0, -10.0, 2.0, 255, 0, 0 );
    arrayTargets[3] = new Target();
    arrayTargets[4] = new Target();
}





///////////////////////
// TESTING FUNCTIONS //
///////////////////////








// function to draw ray from eye to viewplane FOR TESTING PURPOSES ONLY
void GameState::testDrawShot()
{
	const float eyeX = game->getCamera()->getPosX();
	const float eyeY = game->getCamera()->getPosY();
	const float eyeZ = game->getCamera()->getPosZ();

	float dx = clickX - eyeX;
	float dy = clickY - eyeY;
	float dz = clickZ - eyeZ;

	glColor3f(0,1,1);
	glLineWidth(5);

	glBegin(GL_LINE_STRIP);
		glVertex3f(eyeX, eyeY, eyeZ);
		glVertex3f(eyeX + (10 * dx), eyeY + (10 *dy), eyeZ + (10 * dz));
	glEnd();
}


static Vector projectilePos(0,0,0);
static Vector projectileVel(0,0,0);

// funtion to draw "projectile" FOR TESTING PURPOSES ONLY
void GameState::testDrawProjectile()
{
	const float eyeX = game->getCamera()->getPosX();
	const float eyeY = game->getCamera()->getPosY();
	const float eyeZ = game->getCamera()->getPosZ();

	float dx = clickX - eyeX;
	float dy = clickY - eyeY;
	float dz = clickZ - eyeZ;

	float magnitude = sqrt(dx * dx + dy * dy + dz * dz);

	dx /= magnitude;
	dy /= magnitude;
	dz /= magnitude;

	Vector position(projectileStep * dx, projectileStep * dy + 5, projectileStep * dz + 30); // TODO: make "actual" coordinates of projectile

	glColor3f(0,1,0);

	glPushMatrix();
		glTranslatef(eyeX, eyeY, eyeZ); // TODO: make "actual" coordinates of projectile
		for ( int i=0; i<TARGET_COUNT; i++ )
		{
			// TODO: make target z value positive
			if ( (position - Vector(arrayTargets[i]->getCenterX(), arrayTargets[i]->getCenterY(), -arrayTargets[i]->getCenterZ() )).magnitude() < 1 )
			{
				std::cout << "HIT!\n";
				arrayTargets[ i ]->setIsHit( true );
			}
		}
		glTranslatef(projectileStep * dx, projectileStep * dy, projectileStep * dz);
		glutSolidSphere(1, 10, 10);
	glPopMatrix();
	projectileStep += 3;
}