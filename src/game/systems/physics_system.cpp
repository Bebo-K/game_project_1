#include "physics_system.h"

void ApplyGravity(Entity* e, int ms){

}

void ApplySlidingVelocityDampening(Entity* e, int ms){

}
void ApplyMidairVelocityDampening(Entity* e, int ms){

}

void PhysicsSystem::Update(Entity* e,Scene* scene, int ms){
    PhysicsData* phys = e->phys_data;
    if(phys== nullptr)return;
    if(phys->apply_gravity){
        ApplyGravity(e,ms);
    }
    if(phys->world_collision_enabled) {
        //LevelCollision.Update(state, e, delta);
    }
    if(phys->dampen_velocity){
        if(phys->is_midair){
            ApplyMidairVelocityDampening(e,ms);
        }
        else {
            ApplySlidingVelocityDampening(e,ms);
        }
    }
}