#include "phys_components.h"

PhysicsData::PhysicsData():world_hitsphere(1.0f,1.0f){}
PhysicsData::~PhysicsData(){}

ColliderSet::~ColliderSet(){}

MovementData::MovementData():move_goal(0,0,0){
    base_speed = 1.0f;
    jump_speed = 20.0f;
    turn_speed = 800.0f;
    multiplier = 1.0f;
    ramp_up = 1.0f;
    can_move=true;
    is_moving=false;
    can_jump=false;
    is_jumping=false;
    jump_goal=false;
}
MovementData::~MovementData(){}