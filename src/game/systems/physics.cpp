#include "physics.h"

void ApplyGravity(Entity* e, int ms){

}

void ApplySlidingVelocityDampening(Entity* e, int ms){

}
void ApplyMidairVelocityDampening(Entity* e, int ms){

}

void Physics::Update(Scene* scene, int ms){
    for(Entity* e:scene->entities){
    if(e->phys_data== nullptr)continue;
    PhysicsData* phys = e->phys_data;
  
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


}