#ifndef PHYSICS_PROPERTIES_H
#define PHYSICS_PROPERTIES_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/phys/collision_types.hpp>

typedef int CollisionType;

//Component for entities that physics act upon
class PhysicsProperties: public Component{
    public:
    Ellipse_t world_hitsphere;
    CollisionType collision_type = 0;//handlers for world/entity collision
	bool	world_collision_enabled = true;
	
	bool	apply_gravity = true;
	bool	lock_rotation = false;//todo: implement ragdoll phys in a million years.
    bool    dampen_velocity = true;
	float	midair_velocity_damper = 0.75f;
	float	ground_velocity_damper = 0.9999f;
	float	y_velocity_damper = 0;

    PhysicsProperties();
    ~PhysicsProperties();

    Component* Clone();
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

#endif