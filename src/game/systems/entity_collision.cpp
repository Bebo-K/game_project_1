#include "entity_collision.h"



void EntityCollision::Update(Scene* scene, int ms){
    for(Entity* e:scene->entities){
        if(e->colliders == null)continue;
        for(int i=scene->entities.IndexOf(e)+1;i < scene->entities.end().index; i=scene->entities.next(i)){
            Entity* e2 = scene->entities[i];
            if(e2->colliders == null)continue;



        }
    }
}