#include <game_project_1/component/phys_components.hpp>

PhysBody::PhysBody():world_hitsphere(1.0f,1.0f){}
PhysBody::~PhysBody(){}
vec3 PhysBody::GetPosition(){return {*xp,*yp,*zp};}
void PhysBody::SetPosition(vec3 new_pos){*xp = new_pos.x;*yp = new_pos.y;*zp = new_pos.z;}
vec3 PhysBody::GetVelocity(){return {*vxp,*vyp,*vzp};}
void PhysBody::SetVelocity(vec3 new_vel){*vxp = new_vel.x;*vyp = new_vel.y;*vzp = new_vel.z;}
bool PhysBody::IsInBounds(){}
void PhysBody::SetInBounds(bool in_bounds){}
bool PhysBody::IsMidair(){}
void PhysBody::SetMidair(bool midair){}

ColliderSet::~ColliderSet(){}

MovementData::MovementData():move_goal(0,0,0){
    lock_move=false;
    lock_jump=false;
    lock_action=false;
    lock_camera=false;

	base_speed = 1.0f;
	jump_speed = 20.0f;

    jump_hold_boost =25.0f;

	accel_speed = 80.0f;
    jump_accel_speed = 20.0f;
    
	multiplier = 1.0f;

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

float MovementData::MaxSpeed(){return base_speed*multiplier;}