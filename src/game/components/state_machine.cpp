#include "state_machine.h"

/*
StateMachine::StateMachine(String[] layers) {
    layer_names = layers;
    state_names = new String[layers.length][];
    current_state = new StateMask(layer_names.length);
}

void StateMachine::RegisterLayer(int layer_id,String[] layer_states){
    state_names[layer_id]  = layer_states;
}
void StateMachine::SetState(StateMask newstate){current_state = newstate;}
void StateMachine::SetStateLayer(int layer_id,int state_id){current_state.state_ids[layer_id] = state_id;}
void StateMachine::SetStateLayer(int layer_id,String statename){
    int state_id = 0;
    for(int i=0;i<state_names[layer_id].length;i++){
        if(state_names[layer_id][i].compareTo(statename)==0) {state_id =i;}
    }
    current_state.SetState(layer_id, state_id);
}
void StateMachine::SetStateLayer(String layername,String statename){
    int layer_id = 0;
    for(int i=0;i<layer_names.length;i++){
        if(layer_names[i].compareTo(layername)==0) {layer_id =i;}
    }
    int state_id = 0;
    for(int i=0;i<state_names[layer_id].length;i++){
        if(state_names[layer_id][i].compareTo(statename)==0) {state_id =i;}
    }
    
    current_state.SetState(layer_id, state_id);
}
StateMask StateMachine::GetState(){return current_state;}
StateMask StateMachine::NewStateMask(){return new StateMask(layer_names.length);}

StateMask StateMachine::GenStateMask(String[] layers,String[] states) {
    StateMask ret = new StateMask(layer_names.length);
    for(int l=0;l<layers.length;l++) {
        int layer_id = 0;
        for(int i=0;i<layer_names.length;i++){
            if(layer_names[i].compareTo(layers[l])==0) {layer_id =i;}
        }
        int state_id = 0;
        for(int i=0;i<state_names[layer_id].length;i++){
            if(state_names[layer_id][i].compareTo(states[l])==0) {state_id =i;}
        }
        ret.SetState(layer_id, state_id);
    }
    return ret;
}

StateMask StateMachine::GenStateMask(String...states) {
    StateMask ret = new StateMask(layer_names.length);
    for(int layer_id=0;layer_id<layer_names.length;layer_id++) {
        int state_id = 0;
        if(layer_id >= states.length) {break;}
        for(int i=0;i<state_names[layer_id].length;i++){
            if(state_names[layer_id][i].compareTo(states[layer_id])==0) {state_id =i;}
        }
        ret.SetState(layer_id, state_id);
    }
    return ret;
}

StateMask StateMachine::QuickgenStateMask(int[] state_ids){
    StateMask ret = new StateMask(layer_names.length);
    ret.state_ids = state_ids;
    return ret;
}

StateMachine::StateMask(int layers){
    state_ids = new int[layers];
    for(int i=0;i<layers;i++){state_ids[i]=0;}
}

void StateMachine::SetState(int layer_id,int state_id){
    state_ids[layer_id] = state_id;
}

bool StateMachine::Compare(StateMask s2){
    bool ret= true;
    if(s2.state_ids.length != state_ids.length)return false;
    for(int i=0;i<state_ids.length;i++){
        if(state_ids[i] != s2.state_ids[i])ret = false;
    }
    return ret;
}

*/
StateMachine::~StateMachine(){}