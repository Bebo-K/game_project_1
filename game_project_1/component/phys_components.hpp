#ifndef PHYS_COMPONENTS_H
#define PHYS_COMPONENTS_H

#include <game_project_1/types/pool.hpp>
#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/phys/collision_types.hpp>
#include <game_project_1/phys/collider.hpp>
#include <game_project_1/io/serializer.hpp>

//Component for entities that physics act upon
class PhysicsProperties{
    public:
    Ellipse_t world_hitsphere;
	bool	world_collision_enabled = true;
	
	bool	apply_gravity = true;
	bool	lock_rotation = false;//todo: implement ragdoll phys in a million years.
    bool    dampen_velocity = true;
	float	midair_velocity_damper = 0.75f;
	float	ground_velocity_damper = 0.9999f;
	float	y_velocity_damper = 0;

    PhysicsProperties();
    PhysicsProperties(PhysicsProperties* copy);
    ~PhysicsProperties();

    inline int ComponentID(){return 2;}
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

//Component container for hitboxes
class ColliderSet : public Pool<ShapeCollider>{
    public:
    float bounds_xz;
    float bounds_y;
    ColliderSet();
    ColliderSet(ColliderSet* copy);
    ~ColliderSet();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

//Information about how an entity can move: move/jump speeds, run/walk, etc. 
class MoveProperties{
	public:
    float	base_speed;
    float	jump_speed;

    float   jump_hold_boost;

    float	accel_speed;
    float   jump_accel_speed;

    float	multiplier;


    float MaxSpeed();
    float WalkSpeed();

    MoveProperties();
    MoveProperties(MoveProperties* copy);
    ~MoveProperties();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

#endif