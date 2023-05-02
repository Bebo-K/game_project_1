#include <game_project_1/component/shared/movement_state.hpp>



MovementState::MovementState(){Clear();}

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

Component* MovementState::Clone(){
    MovementState* copy = new MovementState();
	copy->current_movement = current_movement;
    copy->move_goal = move_goal;
    copy->jump_goal = jump_goal;
    copy->can_move = can_move;
    copy->can_jump = can_jump;
    copy->is_moving = is_moving;
    copy->is_jumping = is_jumping;
    return copy;
}

MovementState::Clear(){
	current_movement=IDLE;
    move_goal={0,0,0};
    jump_goal=false;

    can_move=true;
    can_jump=false;

    is_moving=false;
    is_jumping=false;
}