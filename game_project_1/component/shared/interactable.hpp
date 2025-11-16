#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/types/math_types.hpp>

enum InteractionType{
    PICKUP,
    TALK,
    CUSTOM
};

class Interactable: public Component{
	public:

    float interact_range;
    FloatRange valid_interact_angle;
    InteractionType type;
    int interaction_content_id;//either dialogue ID or script ID to run

    Interactable();
    ~Interactable();

    Component* Clone(ComponentParentContext context);
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);

};






#endif
