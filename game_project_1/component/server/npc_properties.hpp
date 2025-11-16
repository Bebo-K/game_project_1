#ifndef NPC_PROPERTIES_H
#define NPC_PROPERTIES_H

#include <game_project_1/component/component.hpp>


typedef int NPCControllerType;
class NPCProperties: public Component{
    public: 
    NPCControllerType controller_type;

	NPCProperties();
	NPCProperties(int controller);
	~NPCProperties();

    Component* Clone(ComponentParentContext context);
    virtual void Clear();

    inline int SerializedLength(){return sizeof(int);}
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};




#endif