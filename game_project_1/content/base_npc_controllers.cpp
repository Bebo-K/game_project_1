#include <game_project_1/content/base_content.hpp>


void BaseContent::NPCController_Wander(ServerEntity* e, ServerScene* s){
    NPCControllerState* npc_state = e->npc_state;
    if(!npc_state->init){   
        npc_state->current_action=0;
        npc_state->parameters.Allocate(1);
        npc_state->parameters[0]->wstr = L"Hello!";
        npc_state->init=true;
    }
    e->npc_state->action_counter++;
    if(e->move_state){
        MovementState* move = e->move_state;
        move->move_goal = vec3(0,0,-1);
        move->move_goal.rotate_y(e->npc_state->action_counter);

        if(e->npc_state->action_counter % 1000 == 0){move->jump_goal=true;}
    }

}