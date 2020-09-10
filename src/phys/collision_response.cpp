#include "collision_response.h"




void CollisionResponse::HandleCollision(Entity* e,Scene* s, CollisionList* collision){
      if(collision->surface == null)return;
      switch(collision->surface->type){   
            case SurfaceType::SOLID: HandleSolid(e,s,collision);break;
            case SurfaceType::DAMAGE :HandleDamage(e,s,collision); break;
            case SurfaceType::DEATH: HandleDeath(e,s,collision); break;
            case SurfaceType::NOCLIP : break;
            case SurfaceType::TRIGGER : HandleTrigger(e,s,collision); break;
            case SurfaceType::WATER : HandleWater(e,s,collision); break;
            default: break;
        }
}

void CollisionResponse::HandleSolid(Entity* e,Scene* s,CollisionList* collision)
{

}

void CollisionResponse::HandleWater(Entity* e,Scene* s,CollisionList* collision)
{

}

void CollisionResponse::HandleDamage(Entity* e,Scene* s,CollisionList* collision){



}

void CollisionResponse::HandleDeath(Entity* e,Scene* s,CollisionList* collision){



}

void CollisionResponse::HandleExit(Entity* e,Scene* s,CollisionList* collision){



}

void CollisionResponse::HandleTrigger(Entity* e,Scene* s,CollisionList* collision){



}