#include "BaseTarget.h"

static const float POINT_LOSS = 0.01;

// default, no rotation
BaseTarget::BaseTarget( Vector center ) :
  GameObject( center )
, m_bForwardMovement( true )
, m_status( TargetStatus::INACTIVE )
, m_pointValue( 0 )
{
}

BaseTarget::BaseTarget( Vector center, Vector rotAxis, float pointValue ) :
  GameObject( center )
, m_bForwardMovement( true )
, m_status( TargetStatus::INACTIVE )
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
	m_center = m_keyFrames[ 0 ];
	m_curFrame = 0;
	m_bForwardMovement = true;
	m_status = TargetStatus::ACTIVE;
	m_curPointValue = m_pointValue;
}

void BaseTarget::update()
{
	m_curPointValue -= POINT_LOSS;

	if ( m_curPointValue <= 0.0 )
	{
		m_status = TargetStatus::INACTIVE;
	}

	// if not stationary target
	if ( m_keyFrames.size() != 1 )
	{
		if ( m_bForwardMovement )
		{
			// if at last frame
			if ( m_curFrame + 1 == m_keyFrames.size() )
			{
				// if not repeating
				if ( !m_bRepeatFrames )
				{
					m_status = TargetStatus::INACTIVE;
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
				if ( ( m_center - nextPos ).squareMagnitude() >= ( m_center - m_keyFrames[ m_curFrame + 1 ] ).squareMagnitude() )
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
					m_status = TargetStatus::INACTIVE;
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
				if ( ( m_center - nextPos ).squareMagnitude() >= ( m_center - m_keyFrames[ m_curFrame - 1 ] ).squareMagnitude() )
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
	// if stationary and not rotating, EARLY RETURN
	else if ( m_rotSpeed == 0.0 )
	{
		return;
	}

	// rotate angle
	m_rotAngle += m_rotSpeed;

	if ( m_rotAngle >= 360 )
	{
		m_rotAngle -= 360;
	}

	// post update function (update normals, etc.)
	postUpdate();
}