#ifndef BASE_TARGET_H
#define BASE_TARGET_H

#include <vector>

#include "GameObject.h"
#include "TargetDefs.h"

// contains the target statuses
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

	float getPointValue() const { return m_pointValue; }

	// manipulate keyframe data
	void addKeyFrame( const Vector& keyFrame );
	void setRepeatFrames( bool repeat ) { m_bRepeatFrames = repeat; }

	// set to initial position
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

	TargetStatus::id m_status;

	bool m_bForwardMovement;
	// all keyframes for target
	std::vector<Vector> m_keyFrames;
	// will it repeat its movement path
	bool m_bRepeatFrames;

	int m_curFrame;
};

#endif