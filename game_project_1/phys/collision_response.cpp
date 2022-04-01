#include <game_project_1/phys/collision_response.hpp>


//CLIENT
void ClientCollisionResponse::HandleCollision(ClientEntity* e,ClientScene* s, CollisionList* collision){
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

void ClientCollisionResponse::HandleSolid(ClientEntity* e,ClientScene* s,CollisionList* collision){/**/}
void ClientCollisionResponse::HandleWater(ClientEntity* e,ClientScene* s,CollisionList* collision){/**/}
void ClientCollisionResponse::HandleDamage(ClientEntity* e,ClientScene* s,CollisionList* collision){/*managed by server (TODO: player client-side damage)*/}
void ClientCollisionResponse::HandleDeath(ClientEntity* e,ClientScene* s,CollisionList* collision){/*managed by server*/}
void ClientCollisionResponse::HandleExit(ClientEntity* e,ClientScene* s,CollisionList* collision){/*managed by server*/}
void ClientCollisionResponse::HandleTrigger(ClientEntity* e,ClientScene* s,CollisionList* collision){/*managed by server*/}


//SERVER
void ServerCollisionResponse::HandleCollision(ServerEntity* e,ServerScene* s, CollisionList* collision){
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

void ClientCollisionResponse::HandleSolid(ServerEntity* e,ServerScene* s,CollisionList* collision)
{

}
void ClientCollisionResponse::HandleWater(ServerEntity* e,ServerScene* s,CollisionList* collision){

}
void ClientCollisionResponse::HandleDamage(ServerEntity* e,ServerScene* s,CollisionList* collision){}
void ClientCollisionResponse::HandleDeath(ServerEntity* e,ServerScene* s,CollisionList* collision){}
void ClientCollisionResponse::HandleExit(ServerEntity* e,ServerScene* s,CollisionList* collision){}
void ClientCollisionResponse::HandleTrigger(ServerEntity* e,ServerScene* s,CollisionList* collision){}