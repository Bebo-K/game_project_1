#include "npc_controller.h"



void NPCController::Update(Scene* scene,int frames){
    for(Entity* e:scene->entities){

        if(e->npc_data != null){
            e->npc_data->patience -= frames;
        }

    }
}