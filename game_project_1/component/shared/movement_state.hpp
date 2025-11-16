#ifndef MOVEMENT_STATE_H
#define MOVEMENT_STATE_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/types/primitives.hpp>

enum MovementType: int{
    NONE,
	IDLE,
	WALKING,
	RUNNING,
	JUMPING,
	FALLING,
	LANDING,
    FLYING,
    SWIMMING
};

//State information for entity movement (other than basic physbody info like velocity)
class MovementState: public Component{
	public:

	MovementType    current_movement;
    vec3	        move_goal;
    bool	        jump_goal;

    bool	        can_move;
    bool	        can_jump;

    bool	        is_moving;
    bool	        is_jumping;

	MovementState();
	~MovementState();

    Component* Clone(ComponentParentContext context);
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};




#endif