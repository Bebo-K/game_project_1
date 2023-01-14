#include <game_project_1/component/state_components.hpp>

MovementState::MovementState(){
	current_movement=0;
    move_goal={0,0,0};
    jump_goal=false;

    can_move=false;
    can_jump=false;

    is_moving=false;
    is_jumping=false;
}

int MovementState::SerializedLength(){
    return sizeof(vec3)+sizeof(int)*2;
}

void MovementState::Read(Deserializer& dat){
    current_movement=dat.GetInt();
    move_goal={dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};

    int flags = dat.GetInt();
    jump_goal= (flags & 1) > 0;
    can_move= (flags & 2) > 0;
    can_jump= (flags & 4) > 0;
    is_moving= (flags & 8) > 0;
    is_jumping= (flags & 16) > 0;
}

void MovementState::Write(Serializer& dat){
    dat.PutInt(current_movement);
    dat.PutFloat(move_goal.x); dat.PutFloat(move_goal.y); dat.PutFloat(move_goal.z);
    int flags =0;
    if(jump_goal){flags |= 1;}
    if(can_move){flags |= 2;}
    if(can_jump){flags |= 4;}
    if(is_moving){flags |= 8;}
    if(is_jumping){flags |= 16;}
    dat.PutInt(flags);
}

void MovementState::Copy(MovementState* p2){

}



ActionState::ActionState(){
    current_action=0;
	action_timer=0;
	action_impulse=false;
}

int ActionState::SerializedLength(){
    return sizeof(int)*3;
}

void ActionState::Read(Deserializer& dat){
    current_action = dat.GetInt();
    action_timer = dat.GetInt();
    action_impulse = (dat.GetInt()!=0);
}

void ActionState::Write(Serializer& dat){
    dat.PutInt(current_action);
    dat.PutInt(action_timer);
    dat.PutInt(action_impulse?1:0);
}

void ActionState::Copy(ActionState* p2){
    current_action = p2->current_action;
    action_timer = p2->action_timer;
    action_impulse = p2->action_impulse;
}

/*
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

int State::SerializedLength(){
    return 2;
}
void State::Write(Serializer& dat){
    dat.PutByte((byte)move_state);
    dat.PutByte((byte)combat_state);
}
void State::Read(Deserializer& dat){
    MovementLayerStateID move = (MovementLayerStateID)dat.GetByte();
    CombatLayerStateID combat = (CombatLayerStateID)dat.GetByte();
    if(move_state != move){move_state=move;state_changed=true;}
    if(combat_state != combat){combat_state=combat;state_changed=true;}
}
void State::Copy(State* s2){
    move_state = s2->move_state;
    combat_state = s2->combat_state;
}

*/