#ifndef PHYS_COMPONENTS_H
#define PHYS_COMPONENTS_H

#include "component.h"
#include "../../structs/list.h"
#include "../../structs/3d_types.h"
#include "../../phys/collider.h"

class PhysicsData : Component{
    public:

	float	world_collision_width = 1.0f;
	float	world_collision_height = 1.0f;
	bool	world_collision_enabled = true;
	
	bool	apply_gravity = true;
	bool	lock_rotation = false;//todo: implement ragdoll phys in a million years.
    bool    dampen_velocity = true;
	float	midair_velocity_damper = 0.75f;
	float	ground_velocity_damper = 0.9999f;
	float	y_velocity_damper = 0;
	bool	is_midair	=	false;
	bool    out_of_bounds = false;

    ~PhysicsData();
};

class ColliderSet : List<Collider> , Component{
    public:

    ~ColliderSet();
};

class Movement : Component {
	public:

    float	base_speed;
    float	jump_speed;
    float	multiplier;
    float	ramp_up;

    bool	can_move;
    bool	is_moving;
    vec3	move_goal;
    bool	can_jump;
    bool	is_jumping;
    bool	jump_goal;

    Movement();
    ~Movement();
};




#endif