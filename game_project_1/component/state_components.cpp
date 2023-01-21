#include <game_project_1/component/state_components.hpp>

PhysicsState::PhysicsState(){
    in_bounds=true;
    midair=false;
}
PhysicsState::PhysicsState(PhysicsState* p2){
    in_bounds=p2->in_bounds;
    midair=p2->in_bounds;
}
PhysicsState::~PhysicsState(){}
int PhysicsState::SerializedLength(){
    return sizeof(byte);
}
void PhysicsState::Read(Deserializer& dat){
    byte flags = dat.GetByte();
    in_bounds = (flags & 1)!=0;
    midair = (flags & 2)!=0;
}
void PhysicsState::Write(Serializer& dat){
    byte flags = 0;
    if(in_bounds)flags |=1;
    if(midair)flags |=2;
    dat.PutByte(flags);
}


MovementState::MovementState(){
	current_movement=0;
    move_goal={0,0,0};
    jump_goal=false;

    can_move=false;
    can_jump=false;

    is_moving=false;
    is_jumping=false;
}
MovementState::MovementState(MovementState* m2){
	current_movement=m2->current_movement;
    move_goal=m2->move_goal;
    jump_goal=m2->jump_goal;

    can_move=m2->can_move;
    can_jump=m2->can_jump;

    is_moving=m2->is_moving;
    is_jumping=m2->is_jumping;
}
MovementState::~MovementState(){}
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


ActionState::ActionState(){
    current_action=0;
	action_timer=0;
	action_impulse=false;
}
ActionState::ActionState(ActionState* a2){
    current_action = a2->current_action;
    action_timer = a2->action_timer;
    action_impulse = a2->action_impulse;
}
ActionState::~ActionState(){}
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