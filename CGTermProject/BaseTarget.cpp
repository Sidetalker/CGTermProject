#include "BaseTarget.h"


BaseTarget::BaseTarget( Vector center ) :
  GameObject( center )
, m_bIsHit( false )
{
}


BaseTarget::~BaseTarget()
{
}
