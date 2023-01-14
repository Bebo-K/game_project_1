#ifndef STATE_H
#define STATE_H

#include <game_project_1/io/serializer.hpp>
#include <game_project_1/types/3d_types.hpp>

namespace MovementTypeID{
	const int IDLE =0,
	WALKING=1,
	RUNNING=2,
	JUMPING=3,
	FALLING=4,
	LANDING=5;
};

namespace ActionID{
	const int NONE=0,
	DAMAGED=1,
	DEAD=2,
	ATTACK=3,
	TALK=4,
	PICKUP=5;
};

//State information for entity movement (other than basic physbody info like velocity)
struct MovementState{
	int		current_movement;
    vec3	move_goal;
    bool	jump_goal;

    bool	can_move;
    bool	can_jump;

    bool	is_moving;
    bool	is_jumping;

	MovementState();
	int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Copy(MovementState* p2);
};

//State information for entity actions (attacking, taking damage, talking)
struct ActionState{
	int		current_action;
	int		action_timer;
	bool	action_impulse;
	//int target_entity
	//vec3 target_direction

	int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Copy(ActionState* p2);
};

/*
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
*/

#endif