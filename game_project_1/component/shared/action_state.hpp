#ifndef STATE_H
#define STATE_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/timestep.hpp>

namespace Action{
	enum ID : int{
		NONE=0,
		DAMAGED=1,
		DEAD=2,
		ATTACK=3,
		TALK=4,
		PICKUP=5
	};
}


//State information for entity actions (attacking, taking damage, talking)
class ActionState: public Component{
	public:
	
	Action::ID action_id;
	Timer	action_cooldown;//how long until another action can be taken once this one ends
	Timer	action_timer;//how many frames remain in this action 
	bool	action_impulse;//was an action started this frame
	//int target_entity
	//vec3 target_direction

	ActionState();
	~ActionState();

    Component* Clone(ComponentParentContext context);
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};


#endif