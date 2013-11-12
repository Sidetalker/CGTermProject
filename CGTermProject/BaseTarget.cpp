#include "BaseTarget.h"


BaseTarget::BaseTarget( float centerX, float centerY, float centerZ ) :
  GameObject( centerX, centerY, centerZ )
, m_bIsHit( false )
{
}


BaseTarget::~BaseTarget()
{
}
