#include <game_project_1/system/shared/entity_collision.hpp>
#include <game_project_1/phys/collider.hpp>
#include <game_project_1/types/map.hpp>

#include <game_project_1/component/shared/collider_set.hpp>

using namespace EntityCollision;

Dictionary<CollisionType,ClientHandler> ClientCollisionHandlers;
Dictionary<CollisionType,ServerHandler> ServerCollisionHandlers;

CollisionResult EntityCollision::CheckCollision(Entity* e1, Entity* e2){
    if(!e1->Has<ColliderSet>() || !e2->Has<ColliderSet>())return CollisionResult(false);
    vec3 point={0,0,0};
    ColliderSet* e1_coll = e1->Get<ColliderSet>();
    ColliderSet* e2_coll = e2->Get<ColliderSet>();
    for(ShapeCollider* c1:(*e1_coll)){
        for(ShapeCollider* c2:(*e2_coll)){
            if(c1->Intersects(e1->GetLocation(),e1->scale,c2,e2->GetLocation(),e2->scale,&point)){
                return CollisionResult(point,e1_coll->IndexOf(c1),e2_coll->IndexOf(c2));
            }
        }
    }
    return CollisionResult(false);
}

void EntityCollision::ClientFrame(ClientEntity* e1,ClientScene* s, Timestep delta){
    for(int i2 = s->entities.next(s->entities.IndexOf(e1));i2 != s->entities.end().index;i2 = s->entities.next(i2)){
        ClientEntity* e2 = s->entities[i2];
        CollisionResult res = CheckCollision(e1,e2);
        if(res.collided){
            ColliderSet* e1_coll = e1->Get<ColliderSet>();
            ColliderSet* e2_coll = e2->Get<ColliderSet>();
            int e1_callback = e1_coll->entity_collision_handler_id;
            int e2_callback = e2_coll->entity_collision_handler_id;
            ShapeCollider* e1_hit_collider = (*e1_coll)[res.collider_index_1];
            ShapeCollider* e2_hit_collider = (*e2_coll)[res.collider_index_2];
            if(ClientCollisionHandlers.Has(e1_callback)){
                ClientHandler handler = ClientCollisionHandlers.Get(e1_callback);
                handler(e1,e1_hit_collider,e2,e2_hit_collider,s,res.point);
            }
            if(ClientCollisionHandlers.Has(e2_callback)){
                ClientHandler handler = ClientCollisionHandlers.Get(e1_callback);
                handler(e2,e2_hit_collider,e1,e1_hit_collider,s,res.point);
            }
        }
    }
}

void EntityCollision::ServerFrame(ServerEntity* e1,ServerScene* s, Timestep delta){
    for(int i2 = s->entities.next(s->entities.IndexOf(e1));i2 != s->entities.end().index;i2 = s->entities.next(i2)){
        ServerEntity* e2 = s->entities[i2];
        CollisionResult res = CheckCollision(e1,e2);
        if(res.collided){
            ColliderSet* e1_coll = e1->Get<ColliderSet>();
            ColliderSet* e2_coll = e2->Get<ColliderSet>();
            int e1_callback = e1_coll->entity_collision_handler_id;
            int e2_callback = e2_coll->entity_collision_handler_id;
            ShapeCollider* e1_hit_collider = (*e1_coll)[res.collider_index_1];
            ShapeCollider* e2_hit_collider = (*e2_coll)[res.collider_index_2];
            if(ServerCollisionHandlers.Has(e1_callback)){
                ServerHandler handler = ServerCollisionHandlers.Get(e1_callback);
                handler(e1,e1_hit_collider,e2,e2_hit_collider,s,res.point);
            }
            if(ServerCollisionHandlers.Has(e2_callback)){
                ServerHandler handler = ServerCollisionHandlers.Get(e1_callback);
                handler(e2,e2_hit_collider,e1,e1_hit_collider,s,res.point);
            }
        }
    }
}


CollisionResult::CollisionResult(bool collided){
    this->collided=collided;
    point={0,0,0};
    collider_index_1=collider_index_2=0;
}
CollisionResult::CollisionResult(vec3 intersect,int indx_1,int indx_2){
    collided=true;
    point=intersect;
    collider_index_1=indx_1;
    collider_index_2=indx_2;
}

void EntityCollision::RegisterClientLevelCollisionHandler(CollisionType coll_type, ClientHandler client_callback){
    if(coll_type <= 0) return;
    ClientCollisionHandlers.Add(coll_type,client_callback);
}

void EntityCollision::RegisterServerLevelCollisionHandler(CollisionType coll_type, ServerHandler server_callback){
    if(coll_type <= 0)return;
    ServerCollisionHandlers.Add(coll_type,server_callback);
}
