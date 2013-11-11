#ifndef BASE_TARGET_H
#define BASE_TARGET_H

#include "GameObject.h"

class BaseTarget : public GameObject
{
public:
	BaseTarget( float x, float y, float z );
	~BaseTarget();

};

#endif