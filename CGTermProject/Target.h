#ifndef H_TARGET
#define H_TARGET

#include "BaseTarget.h"
#include "Plane.h"

// Target class.
class Target : public BaseTarget
{
public:
    Target();
    Target( Vector center, float r, unsigned char colorR,
             unsigned char colorG, unsigned char colorB );
    float getRadius()  { return m_radius; }
    virtual void draw();
    
private:
	float m_radius;

	Plane m_nearPlane;
	Plane m_farPlane;

    unsigned char color[ 3 ];
};

#endif