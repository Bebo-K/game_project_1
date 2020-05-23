#include "state.h"






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