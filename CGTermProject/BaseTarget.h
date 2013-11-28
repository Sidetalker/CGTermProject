#ifndef BASE_TARGET_H
#define BASE_TARGET_H

#include "GameObject.h"
#include "TargetDefs.h"

class BaseTarget : public GameObject
{
public:
	BaseTarget( Vector center );
	BaseTarget( Vector center, Vector rotAxis, float rotAngle, uint pointValue );
	virtual ~BaseTarget();

	TargetTypes::id getType() const { return m_type; }
	bool getIsHit() const { return m_bIsHit; }
	void setIsHit( const bool isHit ) { m_bIsHit = isHit; }

	uint getPointValue() const { return m_pointValue; }

protected:
	TargetTypes::id m_type;

	float m_rotAngle;
	Vector m_rotAxis;

private:
	uint m_pointValue;
	bool m_bIsHit;
};

#endif