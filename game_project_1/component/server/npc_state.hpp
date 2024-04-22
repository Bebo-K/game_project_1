#ifndef NPC_STATE_H
#define NPC_STATE_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/arrays.hpp>

//NPC "brains"
class NPCState: public Component{
    public: 
    bool init;//1-time-init control path flag that the controller callback can optionally use
    int current_action;
    int action_counter;
    
    Array<value> constants;
    Array<value> parameters;

	NPCState();
	~NPCState();

    Component* Clone();
    virtual void Clear();

    //Cannot be serialized as we can't know what constants/paramters will be (possibly void*)
    inline int SerializedLength(){return 0;}
    inline void Read(Deserializer& dat){}
    inline void Write(Serializer& dat){}
};

#endif