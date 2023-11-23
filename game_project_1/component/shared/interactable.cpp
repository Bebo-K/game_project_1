
#include <game_project_1/component/shared/interactable.hpp>


Interactable::Interactable(){Clear();}

Interactable::~Interactable(){Clear();}

int Interactable::SerializedLength(){return sizeof(Interactable);}

void Interactable::Read(Deserializer& dat){
    type = (InteractionType)dat.GetInt();
    interaction_content_id = dat.GetInt();
    interact_range = dat.GetFloat();
    valid_interact_angle = FloatRange(dat.GetFloat(),dat.GetFloat());

}

void Interactable::Write(Serializer& dat){
    dat.PutInt(type);
    dat.PutInt(interaction_content_id);
    dat.PutFloat(interact_range);
    dat.PutFloat(valid_interact_angle.min);
    dat.PutFloat(valid_interact_angle.max);
}

Component* Interactable::Clone(){
    Interactable* copy = new Interactable();
    
    copy->type=type;
    copy->interaction_content_id = interaction_content_id;
    copy->interact_range = interact_range;
    copy->valid_interact_angle.min=valid_interact_angle.min;
    copy->valid_interact_angle.max=valid_interact_angle.max;

    return copy;
}

void Interactable::Clear(){
    type=PICKUP;
    interaction_content_id = 0;
    interact_range = 0;
    valid_interact_angle.min=0;
    valid_interact_angle.max=360;
}
