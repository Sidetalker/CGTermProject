#ifndef H_TARGET
#define H_TARGET

#include "BaseTarget.h"

// Target class.
class Target : public BaseTarget
{
public:
    Target();
    Target( float x, float y, float z, float r, unsigned char colorR,
             unsigned char colorG, unsigned char colorB );
    float getRadius()  { return m_radius; }
    virtual void draw();
    
private:
	float m_radius;
    unsigned char color[ 3 ];
};

#endif