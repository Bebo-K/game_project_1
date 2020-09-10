#ifndef STATE_H
#define STATE_H

#include "component.h"


enum MovementLayerStateID{
	IDLE,
	WALKING,
	RUNNING,
	JUMPING,
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

class State : Component {
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

};


#endif