#include "BaseTarget.h"

// default, no rotation
BaseTarget::BaseTarget( Vector center ) :
  GameObject( center )
, m_bForwardMovement( true )
, m_bIsHit( false )
, m_pointValue( 0 )
{
}

BaseTarget::BaseTarget( Vector center, Vector rotAxis, uint pointValue ) :
  GameObject( center )
, m_bForwardMovement( true )
, m_bIsHit( false )
, m_rotAxis( rotAxis )
, m_pointValue( pointValue )
, m_curFrame( 0 )
{

}


BaseTarget::~BaseTarget()
{
}

void BaseTarget::addKeyFrame( const Vector& keyFrame )
{
	m_keyFrames.push_back( keyFrame );
}

void BaseTarget::reset()
{
	m_rotAngle = 0;
	m_center = Vector( 0.0, 0.0, 0.0 );
	m_curFrame = 0;
	m_bForwardMovement = true;
	m_bIsHit = false;
}

void BaseTarget::update()
{
	// do nothing if stationary target
	if ( m_keyFrames.size() == 1 )
	{
		return;
	}
	else
	{
		if ( m_bForwardMovement )
		{
			// if at last frame
			if ( m_curFrame + 1 == m_keyFrames.size() )
			{
				// if not repeating
				if ( !m_bRepeatFrames )
				{
					m_bIsHit = true;
				}
				// else, reverse
				else
				{
					m_bForwardMovement = false;
				}
			}
			else
			{
				Vector velocity( Vector( m_keyFrames[ m_curFrame + 1 ] - m_keyFrames[ m_curFrame ] ).unit() * m_speed );

				Vector nextPos( m_center + velocity );

				// if going to overshoot
				if ( ( m_center - nextPos ).magnitude() >= ( m_center - m_keyFrames[ m_curFrame + 1 ] ).magnitude() )
				{
					m_center = m_keyFrames[ m_curFrame + 1 ];
					++m_curFrame;
				}
				else
				{
					m_center = nextPos;
				}
			}
		}
		else // backward movement
		{
			if ( m_curFrame == 0 )
			{
				if ( !m_bRepeatFrames )
				{
					m_bIsHit = true;
				}
				else
				{
					m_bForwardMovement = true;
				}
			}
			else
			{
				Vector velocity( Vector( m_keyFrames[ m_curFrame ] - m_keyFrames[ m_curFrame - 1 ] ).unit() * -m_speed );
				
				Vector nextPos( m_center + velocity );

				// if going to overshoot
				if ( ( m_center - nextPos ).magnitude() >= ( m_center - m_keyFrames[ m_curFrame - 1 ] ).magnitude() )
				{
					m_center = m_keyFrames[ m_curFrame - 1 ];
					--m_curFrame;
				}
				else
				{
					m_center = nextPos;
				}
			}
		}
	}
		//Vector velocity( Vector( m_keyFrames[ m_curFrame + 1 ] - m_keyFrames[ m_curFrame ] ).unit() * m_speed );
		//m_center = m_center + velocity;

		// post update function (update normals, etc.)
		postUpdate();
}