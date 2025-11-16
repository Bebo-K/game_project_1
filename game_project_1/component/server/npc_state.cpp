#include <game_project_1/component/server/npc_state.hpp>



NPCState::NPCState(){Clear();}
NPCState::~NPCState(){Clear();}

void NPCState::Clear(){
    init=false;
    current_action=0;
    action_counter=0;
    constants.Destroy();
    parameters.Destroy();
}


Component* NPCState::Clone(ComponentParentContext context){
    NPCState* s2 = new NPCState();
    s2->init=init;
    s2->current_action=current_action;
    s2->action_counter=action_counter;
    s2->constants.Init(constants.length);
    //
    for(int i=0;i<constants.length;i++){(*s2->constants[i])=*constants[i];}
    s2->parameters.Init(parameters.length);
    for(int i=0;i<parameters.length;i++){(*s2->parameters[i])=*parameters[i];}
    //UNSAFE- constants/parameters[i] may be a void* that is not expected to be shared
    return s2;
}