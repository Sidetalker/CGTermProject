#include "BaseTarget.h"
#include "Plane.h"

// Target class.
class Target : public BaseTarget
{
public:
    Target();
    Target( Vector center, float r, float speed );
	Target( Vector center, float speed, float r, Vector rotAxis, float rotSpeed );
	virtual ~Target();

    float getRadius()  { return m_radius; }
    virtual void draw();

	Plane getNearPlane() const { return m_nearPlane; }
	Plane getFarPlane() const { return m_farPlane; }
    
private:
	float m_radius;

	// target is made of two planes (circles)
	Plane m_nearPlane;
	Plane m_farPlane;

    unsigned char color[ 3 ];

	// deals with after update procedures
	virtual void postUpdate();
};
