#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

#include "ArrowProjectile.h"
#include "GameState.h"
#include "Globals.h"
#include "Game.h"
#include "Vector.h"
#include "Crosshair.h"
#include "CannonballProjectile.h"
#include "SpreadProjectile.h"
#include "RayProjectile.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

static const int SPREAD_PRECISION = 10;
static const int HALF_SPREAD_PRECISION = SPREAD_PRECISION >> 1;
static const float SPREAD_FACTOR = 0.5;
static const uint NUM_SPREAD = 10;

// outermost scene rotation FOR TESTING PURPOSES ONLY
float Yangle = 0;

// global step counter just FOR TESTING PURPOSES ONLY
float projectileStep = 0.0;

using namespace std;

GameState::GameState() :
  m_curProjectile( ProjectileTypes::SPREAD )
, m_numTargets( TARGET_COUNT ) // TODO: remove TARGET_COUNT dependency
, m_score( 0 )
, clickX( 0 )
, clickY( 0 )
, clickZ( 0 )
, m_pCrosshair( new Crosshair() )
, m_activeProjectiles()
{
	// seed system time into the generator
	srand( ( unsigned )time( 0 ) );

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
	for ( std::list< BaseProjectile* >::iterator i = m_activeProjectiles.begin(); i != m_activeProjectiles.end(); ++i )
	{
		delete ( *i );
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
    
	updateActiveProjectiles();
	drawActiveProjectiles();
	drawHUD();


    // Draw targets
    for (int i = 0; i < m_numTargets; i++)
    {
		if ( !arrayTargets[ i ]->getIsHit() )
			arrayTargets[i]->draw();
    }
    
	// Draw crosshair
	m_pCrosshair->draw();

	// FOR TESTING PURPOSES ONLY draw ray from eye through point clicked
	testDrawShot();

    glFlush();
}

void GameState::mouseAction( int button, int state, int x, int y ) // TODO: cleanup using Vector class instead of many floats
{
	const float eyeX = game->getCamera()->getPosX();
	const float eyeY = game->getCamera()->getPosY();
	const float eyeZ = game->getCamera()->getPosZ();

	const Vector eyePos( eyeX, eyeY, eyeZ );

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
		clickX = ( x + ( ( eyeX / ( frustumHalfWidth ) ) * ( windowHalfWidth ) - ( windowHalfWidth ) ) ) / ( windowWidth / frustumWidth );
		clickY = -( y - ( ( eyeY / ( frustumHalfHeight ) ) * ( windowHalfHeight ) + ( windowHalfHeight ) ) ) / ( windowHeight / frustumHeight );
		clickZ = eyeZ - frustumNear;

		switch ( m_curProjectile )
		{
			case ProjectileTypes::RAY:
			{
				BaseProjectile* p = new RayProjectile( eyePos );
				p->setVelocity( ( Vector( clickX, clickY, clickZ ) - eyePos ) );
				m_activeProjectiles.push_back( p );
				break;
			}
			case ProjectileTypes::CANNONBALL:
			{
				BaseProjectile* p = new CannonballProjectile( eyePos );
				p->setVelocity( ( Vector( clickX, clickY, clickZ ) - eyePos ) );
				m_activeProjectiles.push_back( p );
				break;
			}
			case ProjectileTypes::ARROW:
			{
				BaseProjectile* p = new ArrowProjectile( eyePos );
				p->setVelocity( ( Vector( clickX, clickY, clickZ ) - eyePos ) );
				m_activeProjectiles.push_back( p );
				break;
			}
			case ProjectileTypes::SPREAD:
			{
				// TODO: figure out randomness
				for ( uint i = 0; i < NUM_SPREAD; ++i )
				{
					// generates random float between -1 and 1
					const float x = ( ( float )( ( rand() % ( SPREAD_PRECISION ) ) - HALF_SPREAD_PRECISION ) ) / HALF_SPREAD_PRECISION;
					const float y = ( ( float )( ( rand() % ( SPREAD_PRECISION ) ) - HALF_SPREAD_PRECISION ) ) / HALF_SPREAD_PRECISION;

					BaseProjectile* p = new SpreadProjectile( eyePos );
					p->setVelocity( ( Vector( clickX + ( x * SPREAD_FACTOR ), clickY + ( y * SPREAD_FACTOR ), clickZ ) - eyePos ) );
					m_activeProjectiles.push_back( p );
				}
				break;
			}
			default:
			{
				// error, invalid type
			}
		}
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
		case 's': // display status of scene FOR TESTING PURPOSES ONLY
			for ( int i = 0; i < TARGET_COUNT; i++ )
			{
				std::cout << "Target " << i << ": " << arrayTargets[i]->getCenterX() << " " << arrayTargets[i]->getCenterY() << " " << arrayTargets[i]->getCenterZ() << std::endl;
			}
			break;
		case 'r': // reset targets FOR TESTING PURPOSES ONLY
			for ( uint i = 0; i < TARGET_COUNT; ++i )
			{
				arrayTargets[ i ]->setIsHit( false );
			}
			break;
		case 'd': // cycle forward through projectiles
		{
			m_curProjectile = ( ProjectileTypes::id )( ( int )m_curProjectile + 1 );
			
			// loop back to beginning if necessary
			if ( m_curProjectile == ProjectileTypes::NUM_PROJECTILES )
			{
				m_curProjectile = ( ProjectileTypes::id )0;
			}
			break;
		}
		case 'a': // cycle backward through projectiles
		{
			m_curProjectile = ( ProjectileTypes::id )( ( int )m_curProjectile - 1 );
			
			// loop back to end if necessary
			if ( m_curProjectile == ( ProjectileTypes::id ) -1 )
			{
				m_curProjectile = ( ProjectileTypes::id )( ( int )ProjectileTypes::NUM_PROJECTILES - 1 );
			}
			break;
		}
        default:
            break;
    }
}

void GameState::updateActiveProjectiles()
{
	std::list< BaseProjectile* >::iterator i = m_activeProjectiles.begin();

	// iterate over all projectiles
	while ( i != m_activeProjectiles.end() )
	{
		// if out of frustum bounds
		if ( ( *i )->getCenterZ() < -game->getCamera()->getFrustumFar() )
		{
			// free memory!
			delete ( *i );
			// remove from list
			i = m_activeProjectiles.erase( i );
		}
		// if in bounds
		else
		{
			// apply collision testing and velocity
			( *i )->checkCollisions( arrayTargets, TARGET_COUNT );
			( *i )->applyVelocity();
			++i;
		}
	}
}

void GameState::drawActiveProjectiles()
{
	for ( std::list< BaseProjectile* >::iterator i = m_activeProjectiles.begin(); i != m_activeProjectiles.end(); ++i )
	{
		( *i )->draw();
	}
}

// Initialization routine.
void GameState::setup()
{
	glutSetCursor( GLUT_CURSOR_NONE ); 

    glClearColor(1.0, 1.0, 1.0, 0.0);
    
	// load identity to initialize matrix (prevent unexpected lighting)
	glLoadIdentity();

    // Lighting stuff from checkeredFloor.cpp
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    
    // Turn on OpenGL lighting.
    glEnable(GL_LIGHTING);
    
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
    arrayTargets[0] = new Target( Vector( 0.0, 5.0, 0.0 ), 2.0, 255, 255, 255 );
    arrayTargets[1] = new Target( Vector( 10.0, 5.0, 15.0 ), 2.0, Vector(1,1,0), 90, 255, 255, 255 );
    arrayTargets[2] = new Target( Vector( -10.0, 5.0, 10.0 ), 2.0, 255, 255, 255 );
    arrayTargets[3] = new Target();
    arrayTargets[4] = new Target();
}

void GameState::drawHUD()
{
	// push current MVM
	glPushMatrix();
	// disable lighting
	glDisable( GL_LIGHTING );

	// switch to Ortho projection (with new MVM)
	game->getCamera()->switchToOrtho();

	// arrays to store score c-style strings (allocating 25 chars should be enough)
	char scoreString[25];
	char highScoreString[25];

	// create c-style strings
	sprintf(scoreString,"SCORE: %d", m_score);
	sprintf(highScoreString,"HIGH SCORE: %d", 999999);
	char* projectileType;
		
	// color of text
	glColor3f( 0.0, 0.0, 0.0 );

	// set raster to top left corner (15 is for font height)
	glRasterPos2f( 0, game->getCamera()->getWindowHeight() - 15 );

	// render score
	for ( char* c = scoreString; *c != '\0'; c++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *c );
	}
	
	// set raster to top left corner (15 is for font height)
	glRasterPos2f( 0, game->getCamera()->getWindowHeight() - 30 );
	
	// render high score
	for ( char* c = highScoreString; *c != '\0'; c++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *c );
	}

	// use correct string
	switch ( m_curProjectile )
	{
		case ProjectileTypes::CANNONBALL:
			projectileType = "Cannonball";
			break;
		case ProjectileTypes::SPREAD:
			projectileType = "Spread";
			break;
		case ProjectileTypes::RAY:
			projectileType = "Ray";
			break;
		case ProjectileTypes::ARROW:
			projectileType = "Arrow";
			break;
		default:
			projectileType = "Error, unimplemented type.";
			break;
	}

	// set raster to top right corner (15 is for font height, flushes up against right size)
	glRasterPos2f( game->getCamera()->getWindowWidth() - ( strlen( projectileType ) * 9 ), game->getCamera()->getWindowHeight() - 15 );

	// render projectile type
	for ( char* c = projectileType; *c != '\0'; c++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *c );
	}
    
	// return to Perspective projection (with old PM)
	game->getCamera()->returnToPerspective();
	
	// pop to MVM
	glPopMatrix();
	// re-enable lighting
	glEnable( GL_LIGHTING );
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
