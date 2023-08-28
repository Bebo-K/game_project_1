#include <game_project_1/content/base_content.hpp>

#include <game_project_1/io/log.hpp>
#include <game_project_1/component/server/npc_state.hpp>
#include <game_project_1/component/shared/movement_state.hpp>

void BaseContent::NPCController_Wander(ServerEntity* e, ServerScene* s){
    NPCState* npc_state = e->Get<NPCState>();
    MovementState* move_state = e->Get<MovementState>();
    if(!npc_state->init){   
        npc_state->current_action=0;
        npc_state->parameters.Allocate(1);
        npc_state->parameters[0]->wstr = L"Hello!";
        npc_state->init=true;
    }
    npc_state->action_counter++;
    if(move_state){
        move_state->move_goal = vec3(0,0,-1);
        move_state->move_goal.rotate_y(npc_state->action_counter);
        if(npc_state->action_counter % 1000 == 0){move_state->jump_goal=true;}
    }

}