#ifndef BASE_TARGET_H
#define BASE_TARGET_H

#include <vector>

#include "GameObject.h"
#include "TargetDefs.h"

// contains the game states
struct TargetStatus
{
	enum id
	{
		ACTIVE,
		INACTIVE,
		HIT,
		
		INVALID_STATUS
	};
};

class BaseTarget : public GameObject
{
public:
	BaseTarget( Vector center );
	BaseTarget( Vector center, Vector rotAxis, float pointValue );
	virtual ~BaseTarget();

	TargetTypes::id getType() const { return m_type; }
	TargetStatus::id getStatus() const { return m_status; }
	void setStatus( TargetStatus::id status ) { m_status = status; }

	float getCurPointValue() const { return m_curPointValue; }

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
	float m_pointValue;
	float m_curPointValue;

	TargetStatus::id m_status;

	bool m_bForwardMovement;
	std::vector<Vector> m_keyFrames;
	bool m_bRepeatFrames;

	int m_curFrame;
};

#endif