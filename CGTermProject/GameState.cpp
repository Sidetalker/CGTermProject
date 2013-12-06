#include "GameState.h"

#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
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

static const float TIME_DECREMENT = 0.01;

static const float CLEAR_COLOR[] =
{
	0.0,
	0.149,
	0.565,
	0.0
};

using namespace std;

GameState::GameState() :
  m_curProjectile( ProjectileTypes::SPREAD )
, m_curStage( Stages::INVALID_STAGE )
, m_numTargets( TARGET_COUNT )
, m_playerScore( Score( 0, "" ) )
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

	// clean up OpenGL state machine
	glDisable( GL_LIGHTING );
	glDisable( GL_COLOR_MATERIAL );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
}

// update loop called by StateHandler
void GameState::update()
{
	// camera "eye" data
	const float eyeX = game->getCamera()->getPosX();
	const float eyeY = game->getCamera()->getPosY();
	const float eyeZ = game->getCamera()->getPosZ();

	// camera "look at" data
	const float centerX = game->getCamera()->getCenterX();
	const float centerY = game->getCamera()->getCenterY();
	const float centerZ = game->getCamera()->getCenterZ();
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glLoadIdentity();
    gluLookAt( eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0 );

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

	// flush pipeline
    glFlush();
}

// called by StateHandler callback
void GameState::mouseAction( int button, int state, int x, int y )
{
	// camera "eye" data
	const float eyeX = game->getCamera()->getPosX();
	const float eyeY = game->getCamera()->getPosY();
	const float eyeZ = game->getCamera()->getPosZ();

	const Vector eyePos( eyeX, eyeY, eyeZ );

	// camera projection data
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
		// translate 2D window coordinates to 3D scene on viewplane
		clickX = ( x + ( ( eyeX / ( frustumHalfWidth ) ) * ( windowHalfWidth ) - ( windowHalfWidth ) ) ) / ( windowWidth / frustumWidth );
		clickY = -( y - ( ( eyeY / ( frustumHalfHeight ) ) * ( windowHalfHeight ) + ( windowHalfHeight ) ) ) / ( windowHeight / frustumHeight );
		clickZ = eyeZ - frustumNear;

		// create appropriate projectile in heap
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

	// redraw
	glutPostRedisplay();
}

// called by StateHandler callback
void GameState::mouseMotion( int x, int y )
{
	// camera "eye" data
	const float eyeX = game->getCamera()->getPosX();
	const float eyeY = game->getCamera()->getPosY();
	const float eyeZ = game->getCamera()->getPosZ();

	// camera projection data
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
	
	// set crosshair position
	m_pCrosshair->setCenterX( posX );
	m_pCrosshair->setCenterY( posY );
	m_pCrosshair->setCenterZ( posZ );

	glutPostRedisplay();
}

// Keyboard input processing routine.
void GameState::keyInput( unsigned char key, int x, int y )
{
	if ( m_curStage == Stages::ENTER_INITIALS )
	{
		if ( isalpha( key ) )
		{
			m_playerScore.m_initials += key;
		}
		// EARLY RETURN (ignore all key actions while inputing)
		return;
	}

    switch(key)
    {
        case 27: // exit
		{
            exit(0);
            break;
		}
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
		{
            break;
		}
    }
}

// updates all active projectiles
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

// draws all active projectiles
void GameState::drawActiveProjectiles()
{
	for ( std::list< BaseProjectile* >::iterator i = m_activeProjectiles.begin(); i != m_activeProjectiles.end(); ++i )
	{
		( *i )->draw();
	}
}

// initialization routine
void GameState::setup()
{
	// read data files
	FileReader::readTargets( m_targetData );
	FileReader::readHighScores( m_highScores );

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

// draws HUD elements
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
	sprintf( scoreString,"SCORE: %d", m_playerScore.m_score );
	sprintf( highScoreString,"HIGH SCORE: %d", max( m_playerScore.m_score, m_highScores[ 0 ].m_score ) );

	switch ( m_curStage )
	{
		case Stages::ROUND:
		case Stages::END_ROUND:
		{
			switch ( m_curStage )
			{
				case Stages::ROUND:
				{
					sprintf( roundString,"ROUND: %d OF %d", m_round, NUM_ROUNDS );
					sprintf( timerString,"TIME REMAINING: %d", ( uint )m_timer );
					break;
				}
				case Stages::END_ROUND:
				{
					sprintf( roundString,"ROUND: %d OF %d", m_round + 1, NUM_ROUNDS );
					sprintf( timerString,"GET READY: %d", ( uint )m_timer );
					break;
				}
			}
			break;
		}
		case Stages::EXIT:
		{
			sprintf( roundString,"ROUND: %d OF %d", m_round, NUM_ROUNDS );
			sprintf( timerString, "GAME OVER" );
			break;
		}
		case Stages::ENTER_INITIALS:
		{
			sprintf( roundString,"YOU GOT A HIGH SCORE!" );
			sprintf( timerString, "ENTER INITIALS: %s", m_playerScore.m_initials.c_str() );
			break;
		}
		default:
		{
			sprintf( roundString, "" );
			sprintf( timerString, "" );
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

	if ( m_curStage == Stages::HIGH_SCORES || m_curStage == Stages::OUTRO )
	{
		drawHighScores();
	}

	// return to Perspective projection (with old PM)
	game->getCamera()->returnToPerspective();

	// pop to MVM
	glPopMatrix();
	// re-enable lighting
	glEnable( GL_LIGHTING );
}

// draws high scores to screen
void GameState::drawHighScores()
{
	// enable blending
	glEnable( GL_BLEND );

	// specify blend function
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// draw box on entire screen
	glColor4f( 0.0, 0.0, 0.0, .8 );
	glBegin( GL_POLYGON );
		glVertex2f( 0, 0 );
		glVertex2f( game->getCamera()->getWindowWidth(), 0 );
		glVertex2f( game->getCamera()->getWindowWidth(), game->getCamera()->getWindowHeight() );
		glVertex2f( 0, game->getCamera()->getWindowHeight() );
	glEnd();

	glDisable( GL_BLEND );

	glColor3f( 0.251, 0.969, 0.953 );

	for ( uint i = 0; i < 10; ++i )
	{
		char scoreString[ 25 ];
		sprintf( scoreString, "%d", m_highScores[ i ].m_score );

		// set raster to middle left
		glRasterPos2f( game->getCamera()->getWindowHalfWidth() - ( ( strlen( scoreString ) * 9 ) + 9 ), ( game->getCamera()->getWindowHalfHeight() + ( 15 * 4 ) - ( 15 * i ) ) );

		// render score
		for ( char* c = scoreString; *c != '\0'; c++ )
		{
			glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *c );
		}

		// set raster to middle right
		glRasterPos2f( game->getCamera()->getWindowHalfWidth() + ( 9 ), ( game->getCamera()->getWindowHalfHeight() + ( 15 * 4 ) - ( 15 * i ) ) );

		const char* c = m_highScores[ i ].m_initials.c_str();

		// render initials
		for ( c; *c != '\0'; c++ )
		{
			glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *c );
		}
	}
}

// draws grid floor
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

// draw textured wall
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

// animate floor
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
				m_playerScore.m_score += m_timer;

				// set target as inactive
				arrayTargets[ i ]->setStatus( TargetStatus::INACTIVE );

				// decrement counter
				--m_numActiveTargets;

				break;
			}
		}
    }

	m_timer -= TIME_DECREMENT;

	if ( m_timer <= 0.0 )
	{
		setEndRound();
        
        // deactivate current targets
        for ( uint i = 0; i < m_numTargets; i++ )
        {
            arrayTargets[ i ]->setStatus( TargetStatus::INACTIVE );
        }
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

	m_timer -= TIME_DECREMENT;

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
}

void GameState::updateExit()
{
	// pause briefly
	m_timer -= TIME_DECREMENT;

	if ( m_timer <= 0.0 )
	{
		if ( m_playerScore.m_score > m_highScores.back().m_score ) // should be 9th element
		{
			setEnterInitials();
		}
		else
		{
			setHighScores();
		}
	}
}

void GameState::setEnterInitials()
{
	m_stageUpdate = &GameState::updateEnterInitials;
	m_curStage = Stages::ENTER_INITIALS;
}

void GameState::updateEnterInitials()
{
	if ( m_playerScore.m_initials.length() >= 3 )
	{
		// add player's score to high scores
		m_highScores.push_back( m_playerScore );

		// sort high to low
		sort( m_highScores.begin(), m_highScores.end() );

		// write to file
		FileReader::writeHighScores( m_highScores );

		setHighScores();
	}
}

void GameState::setHighScores()
{
	m_stageUpdate = &GameState::updateHighScores;
	m_curStage = Stages::HIGH_SCORES;

	m_timer = TIME_BETWEEN_ROUNDS * 3;
}

void GameState::updateHighScores()
{
	// pause to see high scores
	m_timer -= TIME_DECREMENT;

	if ( m_timer <= 0.0 )
	{
		setOutro();
	}
}

void GameState::setOutro()
{
	m_stageUpdate = &GameState::updateOutro;
	m_curStage = Stages::OUTRO;
}

void GameState::updateOutro()
{
	// go back to title
	game->getStateHandler()->changeState( StateTypes::TITLESTATE );
}
