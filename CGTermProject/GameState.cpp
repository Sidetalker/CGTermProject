#include "GameState.h"

#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "ArrowProjectile.h"
#include "Globals.h"
#include "Game.h"
#include "Vector.h"
#include "Crosshair.h"
#include "CannonballProjectile.h"
#include "SpreadProjectile.h"
#include "RayProjectile.h"
#include "Textures.h"
#include "TextureDefs.h"
#include "FileReader.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

static const int SPREAD_PRECISION = 10;
static const int HALF_SPREAD_PRECISION = SPREAD_PRECISION >> 1;
static const float SPREAD_FACTOR = 0.5;
static const uint NUM_SPREAD = 10;

static const uint TEXT_PADDING = 5;

static const uint NUM_ROUNDS = 5;
static const float TIME_PER_ROUND = 20.0;
static const float TIME_BETWEEN_ROUNDS = 5.0;

static const float CLEAR_COLOR[] =
{
	0.0,
	0.149,
	0.565,
	0.0
};

// outermost scene rotation FOR TESTING PURPOSES ONLY
float Yangle = 0;

// global step counter just FOR TESTING PURPOSES ONLY
float projectileStep = 0.0;

using namespace std;

GameState::GameState() :
  m_curProjectile( ProjectileTypes::SPREAD )
, m_curStage( Stages::INVALID_STAGE )
, m_numTargets( TARGET_COUNT ) // TODO: remove TARGET_COUNT dependency
, m_score( 0 )
, clickX( 0 )
, clickY( 0 )
, clickZ( 0 )
, m_timer( 0.0 )
, m_bFloorAlphaIncreasing( false )
, m_floorAlpha( 1.0 )
, m_pCrosshair( new Crosshair() )
, m_activeProjectiles()
, m_round( 0 )
, m_numActiveTargets( 0 )
{
	// seed system time into the generator
	srand( ( unsigned )time( 0 ) );

	setup();
	setIntro();
}

GameState::~GameState()
{
	// free crosshair memory
	delete m_pCrosshair;

	// free target memory
	for ( uint i = 0; i < m_targetData.size(); ++i )
	{
		delete m_targetData[ i ];
	}

	// free projectile memory
	for ( std::list< BaseProjectile* >::iterator i = m_activeProjectiles.begin(); i != m_activeProjectiles.end(); ++i )
	{
		delete ( *i );
	}

	glDisable( GL_LIGHTING );
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
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glLoadIdentity();
    gluLookAt( eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0 );

	// TODO: REMOVE: FOR TESTING PURPOSES ONLY
	glRotatef( Yangle, 0.0, 1.0, 0.0 );

	// update changing objects
	updateActiveProjectiles();
	updateFloor();

	// draw scene objects
	drawActiveProjectiles();
	drawFloor();
	drawWalls();
	drawHUD(); // must draw after walls
	m_pCrosshair->draw();

	// call stage update function
	( this->*m_stageUpdate )();

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
				arrayTargets[ i ]->setStatus( TargetStatus::ACTIVE );
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
		if (    ( *i )->getCenterY() < 0
			 || ( *i )->hasHitObject()
			 || ( *i )->getCenterZ() < -game->getCamera()->getFrustumFar()
			 || ( *i )->getCenterZ() > game->getCamera()->getPosZ() )
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
	FileReader::readTargets( m_targetData );
	FileReader::readHighScores( m_highScores );

	glutSetCursor( GLUT_CURSOR_NONE ); 

    glClearColor( CLEAR_COLOR[ 0 ],
	              CLEAR_COLOR[ 1 ],
	              CLEAR_COLOR[ 2 ],
	              CLEAR_COLOR[ 3 ] );
    
	// load identity to initialize matrix (prevent unexpected lighting)
	glLoadIdentity();

    // Lighting stuff from checkeredFloor.cpp
    glEnable( GL_DEPTH_TEST ); // Enable depth testing.
    
    // Turn on OpenGL lighting.
    glEnable( GL_LIGHTING );
    
    // Light property vectors.
    float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
    float lightPos[] = {10.0, 30.0, 25.0, 1.0 };
    float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };
    
    // Light properties.
    glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmb );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec );
    glLightfv( GL_LIGHT0, GL_SPECULAR, lightDifAndSpec );
    glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
    
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
}

void GameState::drawHUD()
{
	// push current MVM
	glPushMatrix();
	// disable lighting
	glDisable( GL_LIGHTING );

	// switch to Ortho projection (with new MVM)
	game->getCamera()->switchToOrtho();

	// enable blending
	glEnable(GL_BLEND);

	// specify blend function
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// draw box for HUD
	glColor4f( 0.0, 0.0, 0.0, .8 );
	glBegin( GL_POLYGON );
		glVertex2f( 0, game->getCamera()->getWindowHeight() - ( 60 + TEXT_PADDING ) );
		glVertex2f( game->getCamera()->getWindowWidth(), game->getCamera()->getWindowHeight() - ( 60 + TEXT_PADDING ) );
		glVertex2f( game->getCamera()->getWindowWidth(), game->getCamera()->getWindowHeight() );
		glVertex2f( 0, game->getCamera()->getWindowHeight() );
	glEnd();

	// disable blending
	glDisable(GL_BLEND);

	// arrays to store score c-style strings (allocating 25 chars should be enough)
	char scoreString[25];
	char highScoreString[25];
	char roundString[25];
	char timerString[30];

	// create c-style strings
	sprintf_s( scoreString,"SCORE: %d", m_score );
	sprintf_s( highScoreString,"HIGH SCORE: %d", max( m_score, m_highScores[ 0 ] ) );

	switch ( m_curStage )
	{
		case Stages::ROUND:
		case Stages::END_ROUND:
		{
			switch ( m_curStage )
			{
				case Stages::ROUND:
				{
					sprintf_s( roundString,"ROUND: %d OF %d", m_round, NUM_ROUNDS );
					sprintf_s( timerString,"TIME REMAINING: %d", ( uint )m_timer );
					break;
				}
				case Stages::END_ROUND:
				{
					sprintf_s( roundString,"ROUND: %d OF %d", m_round + 1, NUM_ROUNDS );
					sprintf_s( timerString,"GET READY: %d", ( uint )m_timer );
					break;
				}
			}
			break;
		}
		case Stages::EXIT:
		{
			sprintf_s( roundString,"ROUND: %d OF %d", m_round, NUM_ROUNDS );
			sprintf_s( timerString, "GAME OVER" );
			break;
		}
		default:
		{
			sprintf_s( roundString, "" );
			sprintf_s( timerString, "" );
			break;
		}
	}

	char* projectileType;

	// use correct string
	switch ( m_curProjectile )
	{
		case ProjectileTypes::CANNONBALL:
			projectileType = "PROJECTILE: Cannonball";
			break;
		case ProjectileTypes::SPREAD:
			projectileType = "PROJECTILE: Spread";
			break;
		case ProjectileTypes::RAY:
			projectileType = "PROJECTILE: Ray";
			break;
		case ProjectileTypes::ARROW:
			projectileType = "PROJECTILE: Arrow";
			break;
		default:
			projectileType = "Error, unimplemented type.";
			break;
	}
	
	// color of text
	glColor3f( 0.251, 0.969, 0.953 );

	// set raster to top left corner (15 is for font height)
	glRasterPos2f( TEXT_PADDING, game->getCamera()->getWindowHeight() - 15 );

	// render score
	for ( char* c = scoreString; *c != '\0'; c++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *c );
	}
	
	// set raster to top left corner (30 is for font height)
	glRasterPos2f( TEXT_PADDING, game->getCamera()->getWindowHeight() - 30 );
	
	// render high score
	for ( char* c = projectileType; *c != '\0'; c++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *c );
	}

	// set raster to top right corner (45 is for font height, flushes up against right size)
	glRasterPos2f( TEXT_PADDING, game->getCamera()->getWindowHeight() - 45 );

	// render projectile type
	for ( char* c = roundString; *c != '\0'; c++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *c );
	}

	// set raster to top right corner (45 is for font height, flushes up against right size)
	glRasterPos2f( TEXT_PADDING, game->getCamera()->getWindowHeight() - 60 );

	// render projectile type
	for ( char* c = timerString; *c != '\0'; c++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *c );
	}

	// set raster to top right corner (15 is for font height, flushes up against right size)
	glRasterPos2f( game->getCamera()->getWindowWidth() - ( ( strlen( highScoreString ) * 9 ) + TEXT_PADDING ), game->getCamera()->getWindowHeight() - 15 );

	// render projectile type
	for ( char* c = highScoreString; *c != '\0'; c++ )
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

void GameState::drawFloor()
{
	glEnable( GL_TEXTURE_2D ); // Enable 2D textures
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE ); // Set texture environment parameters

	// allow for pulsating floor
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f( 0.0, 0.0, 0.0, m_floorAlpha );

	// Draw rectangle with grid
	glBindTexture( GL_TEXTURE_2D, textures->getTextureIndices()[ TextureTypes::GRID ] );      
	glBegin( GL_POLYGON );
		glTexCoord2f( 0.0, 0.0 ); glVertex3f( -100.0, 0.0, 100.0 );
		glTexCoord2f( 30.0, 0.0 ); glVertex3f( 100.0, 0.0, 100.0 );
		glTexCoord2f( 30.0, 30.0 ); glVertex3f( 100.0, 0.0, -100.0 );
		glTexCoord2f( 0.0, 30.0 ); glVertex3f( -100.0, 0.0, -100.0 );
	glEnd();

	glDisable( GL_BLEND );

	glDisable( GL_TEXTURE_2D );
}

void GameState::drawWalls()
{
	glEnable( GL_TEXTURE_2D ); // Enable 2D textures
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE ); // Set texture environment parameters

	// Map the wall texture onto a rectangle parallel to the xy-plane.
	glBindTexture( GL_TEXTURE_2D, textures->getTextureIndices()[ TextureTypes::WALL ] );        
	glBegin( GL_POLYGON );
		glTexCoord2f( 0.0, 0.0 ); glVertex3f( -100.0, 0.0, -60.0 );
		glTexCoord2f( 1.0, 0.0 ); glVertex3f( 100.0, 0.0, -60.0 );
		glTexCoord2f( 1.0, 1.0 ); glVertex3f( 100.0, 200.0, -60.0 );
		glTexCoord2f( 0.0, 1.0 ); glVertex3f( -100.0, 200.0, -60.0 );
	glEnd();

	glDisable( GL_TEXTURE_2D );
}

void GameState::updateFloor()
{
	// handle pulsating floor
	if ( m_bFloorAlphaIncreasing )
	{
		m_floorAlpha += .003;

		if ( m_floorAlpha >= 1.0 )
		{
			m_bFloorAlphaIncreasing = false;
		}
	}
	else
	{
		m_floorAlpha -= .003;

		if ( m_floorAlpha <= 0.1 )
		{
			m_bFloorAlphaIncreasing = true;
		}
	}
}

// game flow functions

void GameState::setIntro()
{
	m_stageUpdate = &GameState::updateIntro;
	m_curStage = Stages::INTRO;
}

void GameState::updateIntro()
{
	setRound();
}

void GameState::setRound()
{
	m_stageUpdate = &GameState::updateRound;
	m_curStage = Stages::ROUND;

	// increase round number
	++m_round;

	// reset timer
	m_timer = TIME_PER_ROUND;

	// TODO: cleanup hacky solution
	// assigns three targets as active
	for ( uint i = 0; i < TARGET_COUNT; ++i )
	{
		uint randIndex;

		do
		{
			randIndex = rand() % m_targetData.size();
		}
		while ( m_targetData[ randIndex ]->getStatus() == TargetStatus::ACTIVE );

		arrayTargets[ i ] = m_targetData[ randIndex ];
		arrayTargets[ i ]->reset();
	}

	m_numActiveTargets = TARGET_COUNT;
}

void GameState::updateRound()
{
	if ( m_numActiveTargets == 0 )
	{
		setEndRound();
		return;
	}

    // draw and update targets
    for ( uint i = 0; i < m_numTargets; i++ )
    {
		switch ( arrayTargets[ i ]->getStatus() )
		{
			// simply draw and update
			case TargetStatus::ACTIVE:
			{
				arrayTargets[i]->draw();
				arrayTargets[i]->update();
				break;
			}
			// reassign
			case TargetStatus::HIT:
			{
				// get points
				m_score += m_timer;

				// set target as inactive
				arrayTargets[ i ]->setStatus( TargetStatus::INACTIVE );

				// decrement counter
				--m_numActiveTargets;

				break;
			}
		}
    }	

	// FOR TESTING PURPOSES ONLY draw ray from eye through point clicked
	//testDrawShot();

	m_timer -= .01; // TODO: unhardcode

	if ( m_timer <= 0.0 )
	{
		setEndRound();
	}

}

void GameState::setEndRound()
{
	m_stageUpdate = &GameState::updateEndRound;
	m_curStage = Stages::END_ROUND;

	for ( uint i = 0; i < TARGET_COUNT; ++i )
	{
		arrayTargets[ i ]->setStatus( TargetStatus::INACTIVE );
	}

	m_timer = TIME_BETWEEN_ROUNDS;
}

void GameState::updateEndRound()
{
	if ( m_round == NUM_ROUNDS )
	{
		setExit();
		return;
	}

	m_timer -= 0.01;

	if ( m_timer <= 0.0 )
	{
		setRound();
	}
}

void GameState::setExit()
{
	m_stageUpdate = &GameState::updateExit;
	m_curStage = Stages::EXIT;

	m_timer = TIME_BETWEEN_ROUNDS;

	m_highScores.push_back( m_score );

	sort( m_highScores.begin(), m_highScores.end(), greater< uint >() );

	FileReader::writeHighScores( m_highScores );
}

void GameState::updateExit()
{
	// go back to title or go to high scores or whatever
	m_timer -= 0.01;

	if ( m_timer <= 0.0 )
	{
		game->getStateHandler()->changeState( StateTypes::TITLESTATE );
	}
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
