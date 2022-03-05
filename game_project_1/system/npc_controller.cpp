#include <game_project_1/system/npc_controller.hpp>

void NPCController::Update(Scene* scene,int frames){
    for(Entity* e:scene->entities){

        if(e->npc_data != null){
            e->npc_data->patience -= frames;
        }

    }
}