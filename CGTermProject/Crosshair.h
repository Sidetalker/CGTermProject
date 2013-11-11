#ifndef H_CROSSHAIR
#define H_CROSSHAIR

#include "GameObject.h"

// TODO: implement tracking crosshair
class Crosshair : public GameObject
{
public:
	Crosshair();
	~Crosshair();
	virtual void draw();
};

#endif