#ifndef BASE_TARGET_H
#define BASE_TARGET_H

#include <vector>

#include "GameObject.h"
#include "TargetDefs.h"

class BaseTarget : public GameObject
{
public:
	BaseTarget( Vector center );
	BaseTarget( Vector center, Vector rotAxis, uint pointValue );
	virtual ~BaseTarget();

	TargetTypes::id getType() const { return m_type; }
	bool getIsHit() const { return m_bIsHit; }
	void setIsHit( const bool isHit ) { m_bIsHit = isHit; }

	uint getPointValue() const { return m_pointValue; }

	void addKeyFrame( const Vector& keyFrame );
	void setRepeatFrames( bool repeat ) { m_bRepeatFrames = repeat; }
	void reset();
	void update();

protected:
	TargetTypes::id m_type;

	float m_rotAngle;
	Vector m_rotAxis;

	float m_speed;
	float m_rotSpeed;

	virtual void postUpdate() = 0;

private:
	uint m_pointValue;
	bool m_bIsHit;

	bool m_bForwardMovement;
	std::vector<Vector> m_keyFrames;
	bool m_bRepeatFrames;

	int m_curFrame;
};

#endif