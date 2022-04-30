#ifndef STATE_H
#define STATE_H

#include <game_project_1/io/serializer.hpp>


enum MovementLayerStateID{
	IDLE,
	WALKING,
	RUNNING,
	JUMPING,
	JUMP_APEX,
	FALLING,
	LANDING
};

enum CombatLayerStateID{
	NOCOMBAT,
	DAMAGED,
	DYING,
	DEAD,
	ATTACKING,
	TALKING
};

class State {
	MovementLayerStateID 	move_state;
	CombatLayerStateID		combat_state;

	public:
	bool 					state_changed=true;

	State();

	bool Is(MovementLayerStateID query);
	void Set(MovementLayerStateID newstate);
	bool Is(CombatLayerStateID query);
	void Set(CombatLayerStateID newstate);

	bool operator==(State* s2);
	bool operator!=(State* s2);
	void operator=(State* s2);

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Copy(State* s2);

};


#endif