#include "movement.h"



void Movement::Update(Scene* scene, int ms){
    for(Entity* e:scene->entities){
        if(e->movement != null){
            e->x += e->movement->move_goal.x * (float)ms/100.0f;
            e->y += e->movement->move_goal.y * (float)ms/100.0f;
            e->z += e->movement->move_goal.z * (float)ms/100.0f;
        }
    }
}