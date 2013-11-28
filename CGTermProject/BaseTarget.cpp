#include "BaseTarget.h"

// default, no rotation
BaseTarget::BaseTarget( Vector center ) :
  GameObject( center )
, m_bIsHit( false )
, m_pointValue( 0 )
{
}

BaseTarget::BaseTarget( Vector center, Vector rotAxis, float rotAngle, uint pointValue ) :
  GameObject( center )
, m_bIsHit( false )
, m_rotAngle( rotAngle )
, m_rotAxis( rotAxis )
, m_pointValue( pointValue )
{

}


BaseTarget::~BaseTarget()
{
}
