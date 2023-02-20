#ifndef PHYS_COMPONENTS_H
#define PHYS_COMPONENTS_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/phys/collision_types.hpp>
#include <game_project_1/phys/collider.hpp>

//Component for entities that physics act upon
class PhysicsProperties: public SharedComponent{
    public:
    static int ComponentID = 2;
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

    inline int ID(){return PhysicsProperties::ComponentID;}
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

//Component container for hitboxes
class ColliderSet: public SharedComponent public Pool<ShapeCollider>{
    public:
    static int ComponentID = 8;

    float bounds_xz;
    float bounds_y;
    ColliderSet();
    ColliderSet(ColliderSet* copy);
    ~ColliderSet();

    inline int ID(){return ColliderSet::ComponentID;}
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

//Information about how an entity can move: move/jump speeds, run/walk, etc. 
class MoveProperties: public SharedComponent{
	public:
    static int ComponentID = 3;

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

    inline int ID(){return MoveProperties::ComponentID;}
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

#endif