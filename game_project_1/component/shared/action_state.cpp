#include <game_project_1/component/shared/action_state.hpp>


ActionState::ActionState(){Clear();}

ActionState::~ActionState(){}

int ActionState::SerializedLength(){
    return sizeof(int)*4;
}

void ActionState::Read(Deserializer& dat){
    action_id = (Action::ID)dat.GetInt();
    action_cooldown = dat.GetInt();
    action_timer = dat.GetInt();
    action_impulse = (dat.GetInt()!=0);
}

void ActionState::Write(Serializer& dat){
    dat.PutInt(action_id);
    dat.PutInt(action_cooldown);
    dat.PutInt(action_timer);
    dat.PutInt(action_impulse?1:0);
}

Component* ActionState::Clone(){
    ActionState* copy = new ActionState();
    copy->action_id = action_id;
    copy->action_cooldown = action_cooldown;
    copy->action_timer = action_timer;
    copy->action_impulse = action_impulse;
    return copy;
}

void ActionState::Clear(){ 
    action_id=Action::NONE;
    action_cooldown=0;
	action_timer=0;
	action_impulse=false;
}