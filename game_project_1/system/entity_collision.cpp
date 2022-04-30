#include <game_project_1/system/entity_collision.hpp>
#include <game_project_1/phys/collider.hpp>
#include <game_project_1/types/map.hpp>

using namespace EntityCollision;

bool EntityClassPair::operator==(EntityClassPair& b){//if one class is set, a second key of '0' is a wildcard
    return  (class_a==b.class_a && (class_b ==0 || class_b == b.class_b)) ||
            (class_b==b.class_a && (class_a ==0 || class_a == b.class_b));
}
bool EntityClassPair::operator==(int b){//for null comparison. Pair {0,0} is null/catchall
    return (class_a==b && class_b==b);
}

Map<EntityClassPair,ClientHandlerCallback> EntityPairClientCollisionHandlers;
Map<EntityClassPair,ServerHandlerCallback> EntityPairServerCollisionHandlers;

CollisionResult EntityCollision::CheckCollision(BaseEntity* e1, BaseEntity* e2){
    if(e1->colliders == null || e2->colliders == null)return CollisionResult(false);
    vec3 point={0,0,0};
    for(ShapeCollider* c1:*(e1->colliders)){
        for(ShapeCollider* c2:*(e2->colliders)){
            if(c1->Intersects(e1->GetLocation(),c2,e2->GetLocation(),&point)){
                return CollisionResult(point,e1->colliders->IndexOf(c1),e2->colliders->IndexOf(c2));
            }
        }
    }
    return CollisionResult(false);
}

void EntityCollision::ClientFrame(ClientScene* s,ClientEntity* e1,float delta){
    for(int i2 = s->entities.next(s->entities.IndexOf(e1));i2 != s->entities.end().index;i2 = s->entities.next(i2)){
        ClientEntity* e2 = s->entities[i2];
        CollisionResult res = CheckCollision(e1,e2);
        if(res.collided){
            EntityClassPair collided_pair = {e1->entity_class_id,e2->entity_class_id};
            int callback_map_index  = EntityPairClientCollisionHandlers.MatchIndex(collided_pair);
            if(callback_map_index >= 0){
                ClientHandlerCallback callback = EntityPairClientCollisionHandlers.Get(collided_pair);
                EntityClassPair expected_pair = EntityPairClientCollisionHandlers.KeyAtIndex(callback_map_index);
                //try to order entities the way they're registered.
                if(expected_pair.class_a == collided_pair.class_a || expected_pair.class_b == collided_pair.class_b){ 
                    callback(e1,(*e1->colliders)[res.collider_index_1],e2,(*e2->colliders)[res.collider_index_2],s,res.point);
                }
                else{
                    callback(e2,(*e2->colliders)[res.collider_index_2],e1,(*e1->colliders)[res.collider_index_1],s,res.point);
                }
            }
        }
    }
}

void EntityCollision::ServerFrame(ServerScene* s,ServerEntity* e1,float delta){
    for(int i2 = s->entities.next(s->entities.IndexOf(e1));i2 != s->entities.end().index;i2 = s->entities.next(i2)){
        ServerEntity* e2 = s->entities[i2];
        CollisionResult res = CheckCollision(e1,e2);
        if(res.collided){
            EntityClassPair collided_pair = {e1->entity_class_id,e2->entity_class_id};
            int callback_map_index  = EntityPairServerCollisionHandlers.MatchIndex(collided_pair);
            if(callback_map_index >= 0){
                ServerHandlerCallback callback = EntityPairServerCollisionHandlers.Get(collided_pair);
                EntityClassPair expected_pair = EntityPairServerCollisionHandlers.KeyAtIndex(callback_map_index);
                //try to order entities the way they're registered.
                if(expected_pair.class_a == collided_pair.class_a || expected_pair.class_b == collided_pair.class_b){ 
                    callback(e1,(*e1->colliders)[res.collider_index_1],e2,(*e2->colliders)[res.collider_index_2],s,res.point);
                }
                else{
                    callback(e2,(*e2->colliders)[res.collider_index_2],e1,(*e1->colliders)[res.collider_index_1],s,res.point);
                }
            }
        }
    }
}


CollisionResult::CollisionResult(bool collided){
    collided=false;
    point={0,0,0};
    collider_index_1=collider_index_2=0;
}
CollisionResult::CollisionResult(vec3 intersect,int indx_1,int indx_2){
    collided=true;
    point=intersect;
    collider_index_1=indx_1;
    collider_index_2=indx_2;
}

void EntityCollision::RegisterClientEntityClassCallbacks(EntityClassPair pair,ClientHandlerCallback client_callback){
    EntityPairClientCollisionHandlers.Add(pair,client_callback);
}

void EntityCollision::RegisterServerEntityClassCallbacks(EntityClassPair pair,ServerHandlerCallback server_callback){
    EntityPairServerCollisionHandlers.Add(pair,server_callback);
}