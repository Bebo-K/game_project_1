#include <game_project_1/system/entity_collision.hpp>
#include <game_project_1/phys/collision_response.hpp>


void EntityCollision::ClientFrameUpdate(ClientEntity* e,ClientScene* s, float delta){
    vec3 collision_point = {0,0,0};
    for(ClientEntity* e1:s->entities){
        if(e1->colliders == null)continue;
        for(int i=s->entities.IndexOf(e1)+1;i < s->entities.end().index; i=s->entities.next(i)){
            ClientEntity* e2 = s->entities[i];
            if(e2->colliders == null)continue;

            for(CapsuleCollider* c1:*(e1->colliders)){
                for(CapsuleCollider* c2:*(e2->colliders)){
                    if(c1->Intersects(e1->GetPos(),c2,e2->GetPos())){
                        ClientCollisionResponse::HandleEntity(e1,e2,s,c1,c2,collision_point);
                    }
                }
            }
        }
    }
}

void EntityCollision::ServerFrameUpdate(ServerEntity* e,ServerScene* s, float delta){
    vec3 collision_point = {0,0,0};
    for(ServerEntity* e1:s->entities){
        if(e1->colliders == null)continue;
        for(int i=s->entities.IndexOf(e1)+1;i < s->entities.end().index; i=s->entities.next(i)){
            ServerEntity* e2 = s->entities[i];
            if(e2->colliders == null)continue;

            for(CapsuleCollider* c1:*(e1->colliders)){
                for(CapsuleCollider* c2:*(e2->colliders)){
                    if(c1->Intersects(e1->GetPos(),c2,e2->GetPos())){
                        ServerCollisionResponse::HandleEntity(e1,e2,s,c1,c2,collision_point);
                    }
                }
            }
        }
    }
}