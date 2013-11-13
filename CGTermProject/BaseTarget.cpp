#include "BaseTarget.h"

// default, no rotation
BaseTarget::BaseTarget( Vector center ) :
  GameObject( center )
, m_bIsHit( false )
{
}

BaseTarget::BaseTarget( Vector center, Vector rotAxis, float rotAngle ) :
  GameObject( center )
, m_bIsHit( false )
, m_rotAngle( rotAngle )
, m_rotAxis( rotAxis )
{

}


BaseTarget::~BaseTarget()
{
}
