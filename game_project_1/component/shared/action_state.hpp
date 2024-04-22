#ifndef STATE_H
#define STATE_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/types/primitives.hpp>

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
	int		action_cooldown;//how long until another action can be taken (-1 for infinite)
	int		action_timer;//how many frames action has been happening 
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