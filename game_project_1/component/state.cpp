#include <game_project_1/component/state.hpp>

State::State(){
    state_changed=true;
}

bool State::operator==(State* s2){
    return move_state == s2->move_state && combat_state == s2->combat_state;
}
bool State::operator!=(State* s2){
    return move_state!=s2->move_state || combat_state != s2->combat_state;
}
void State::operator=(State* s2){
    if(move_state!=s2->move_state || combat_state != s2->combat_state){
        state_changed=true;
    }
    move_state = s2->move_state;
    combat_state = s2->combat_state;
}

bool State::Is(MovementLayerStateID query){
    return query==move_state;
}
bool State::Is(CombatLayerStateID query){
    return query==combat_state;
}
void State::Set(MovementLayerStateID newstate){
    if(move_state!=newstate){state_changed=true;}
    move_state=newstate;
}
void State::Set(CombatLayerStateID newstate){
    if(combat_state!=newstate){state_changed=true;}
    combat_state=newstate;
}