#ifndef BASE_TARGET_H
#define BASE_TARGET_H

#include "GameObject.h"
#include "TargetDefs.h"

class BaseTarget : public GameObject
{
public:
	BaseTarget( float x, float y, float z );
	~BaseTarget();

	Targets::id getType() const { return m_type; }
	bool getIsHit() const { return m_bIsHit; }
	void setIsHit( const bool isHit ) { m_bIsHit = isHit; }

private:
	bool m_bIsHit;

	Targets::id m_type;
};

#endif