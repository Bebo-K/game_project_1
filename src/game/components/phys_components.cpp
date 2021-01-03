#include "phys_components.h"

PhysicsData::PhysicsData():world_hitsphere(1.0f,1.0f){}
PhysicsData::~PhysicsData(){}

ColliderSet::~ColliderSet(){}

MovementData::MovementData():move_goal(0,0,0){
    lock_move=false;
    lock_jump=false;
    lock_action=false;
    lock_camera=false;

	base_speed = 1.0f;
	jump_speed = 20.0f;
    turn_speed = 800.0f;
	multiplier = 1.0f;
	ramp_up = 1.0f;

	can_move = true;
	is_moving = false;
	//move_goal = {0,0,0};

	can_jump = true;
	is_jumping = false;
	jump_goal = false;

    can_action = true;
    is_action = false;
    action_goal = false;

    can_camera = true;
    cam_moving = false;
    cam_rot_goal = 0;
    cam_zoom_goal = 10;
}
MovementData::~MovementData(){}