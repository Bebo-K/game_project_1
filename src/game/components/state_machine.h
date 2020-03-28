#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "../component.h"

class StateMask{
    public:
    /*
    int[]	state_ids;
    //TODO Implement this: public String state_names;
    
    StateMask(int layers);
    
    void SetState(int layer_id,int state_id);
    bool Compare(StateMask s2);
    //TODO: On StateMask, create DeepCompare() that checks that state ids AND names match
    */
};

class StateMachine : Component {	
	public:
	/*
	String[]					layer_names;
	String[][]					state_names;
	StateMask					current_state;
	
	 StateMachine(String[] layers);
	
	void RegisterLayer(int layer_id,String[] layer_states);
	void SetState(StateMask newstate);
	void SetStateLayer(int layer_id,int state_id);
	void SetStateLayer(int layer_id,String statename);
	void SetStateLayer(String layername,String statename);
	StateMask GetState();
	StateMask NewStateMask();

	StateMask GenStateMask(String[] layers,String[] states);
	StateMask GenStateMask(String...states);
	StateMask QuickgenStateMask(int[] state_ids);
    */
    ~StateMachine();
};


#endif