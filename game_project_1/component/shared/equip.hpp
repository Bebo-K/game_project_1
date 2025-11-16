#ifndef EQUIP_H
#define EQUIP_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/game/items.hpp>

 //Only equipment is sent thru client delta
class Equip: public Component{
	public:

    ItemInstance head;
    ItemInstance body;
    ItemInstance gloves;
    ItemInstance boots;
    ItemInstance right_hand;
    ItemInstance left_hand;

    Equip();
    ~Equip();

    Component* Clone(ComponentParentContext context);
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};


#endif