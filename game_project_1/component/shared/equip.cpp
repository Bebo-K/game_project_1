#include <game_project_1/component/shared/equip.hpp>


Equip::Equip(){Clear();}

Equip::~Equip(){}

int Equip::SerializedLength(){return sizeof(ItemInstance)*6;}

void Equip::Read(Deserializer& dat){
    head.Read(dat);
    body.Read(dat);
    gloves.Read(dat);
    boots.Read(dat);
    right_hand.Read(dat);
    left_hand.Read(dat);
}

void Equip::Write(Serializer& dat){
    head.Write(dat);
    body.Write(dat);
    gloves.Write(dat);
    boots.Write(dat);
    right_hand.Write(dat);
    left_hand.Write(dat);
}

Component* Equip::Clone(){
    Equip* copy = new Equip();
    copy->head = head;
    copy->body = body;
    copy->gloves = gloves;
    copy->boots = boots;
    copy->right_hand = right_hand;
    copy->left_hand = left_hand;
    return copy;
}

void Equip::Clear(){
    head.Clear();
    body.Clear();
    gloves.Clear();
    boots.Clear();
    right_hand.Clear();
    left_hand.Clear();
}