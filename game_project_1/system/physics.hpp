#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <game_project_1/system/system.hpp>


namespace Physics{

	extern float GRAVITY_TERMINAL;
	extern float GRAVITY_ACCEL;
	void FrameUpdate(Scene* scene,float frame_interval);

};



#endif