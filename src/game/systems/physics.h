#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../scene.h"


namespace Physics{

	extern float GRAVITY_TERMINAL;
	extern float GRAVITY_ACCEL;
	void FrameUpdate(Scene* scene,float frame_interval);

};



#endif