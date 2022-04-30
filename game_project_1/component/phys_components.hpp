#ifndef PHYS_COMPONENTS_H
#define PHYS_COMPONENTS_H

#include <game_project_1/types/pool.hpp>
#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/phys/collision_types.hpp>
#include <game_project_1/phys/collider.hpp>
#include <game_project_1/io/serializer.hpp>

class PhysBody{
    public:
    Ellipse_t world_hitsphere;
	bool	world_collision_enabled = true;
	
	bool	apply_gravity = true;
	bool	lock_rotation = false;//todo: implement ragdoll phys in a million years.
    bool    dampen_velocity = true;
	float	midair_velocity_damper = 0.75f;
	float	ground_velocity_damper = 0.9999f;
	float	y_velocity_damper = 0;
	bool	is_midair	=	false;
	bool    out_of_bounds = false;

    PhysBody();
    ~PhysBody();

    vec3 GetPosition();
    void SetPosition(vec3 new_pos);
    vec3 GetVelocity();
    void SetVelocity(vec3 new_vel);
    bool IsInBounds();
    void SetInBounds(bool in_bounds);
    bool IsMidair();
    void SetMidair(bool midair);

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Copy(PhysBody* p2);
};

class ColliderSet : public Pool<ShapeCollider>{
    public:
    float bounds_xz;
    float bounds_y;
    ~ColliderSet();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Copy(ColliderSet* p2);
};

class MovementData{
	public:
    bool    lock_move;
    bool    lock_jump;
    bool    lock_action;

    float	base_speed;
    float	jump_speed;

    float   jump_hold_boost;

    float	accel_speed;
    float   jump_accel_speed;

    float	multiplier;

    bool	can_move;
    bool	is_moving;
    vec3	move_goal;

    bool	can_jump;
    bool	is_jumping;
    bool	jump_goal;

    bool    can_action;
    bool    is_action;
    bool    action_goal;

    float MaxSpeed();

    MovementData();
    ~MovementData();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Copy(MovementData* p2);
};




#endif