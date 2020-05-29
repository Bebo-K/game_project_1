#include "state.h"

bool State::operator==(State* s2){
    return move_state == s2->move_state && combat_state == s2->combat_state;
}
bool State::operator!=(State* s2){
    return move_state!=s2->move_state || combat_state != s2->combat_state;
}
void State::operator=(State* s2){
    move_state = s2->move_state;
    combat_state = s2->combat_state;
}

bool State::Is(MovementLayerStateID query){
    return query==move_state;
}
void State::Set(MovementLayerStateID newstate){
    move_state=newstate;
}
bool State::Is(CombatLayerStateID query){
    return query==combat_state;
}
void State::Set(CombatLayerStateID newstate){
    combat_state=newstate;
}