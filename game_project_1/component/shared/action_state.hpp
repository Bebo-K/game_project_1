#ifndef STATE_H
#define STATE_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/types/3d_types.hpp>


namespace ActionID{
	const int NONE=0;
	const int DAMAGED=1;
	const int DEAD=2;
	const int ATTACK=3;
	const int TALK=4;
	const int PICKUP=5;
};//TODO: extensible enum

//State information for entity actions (attacking, taking damage, talking)
class ActionState: public Component{
	public:
	
	int		action_id;
	int		action_timer;
	bool	action_impulse;
	//int target_entity
	//vec3 target_direction

	ActionState();
	~ActionState();

    Component* Clone();
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};


#endif