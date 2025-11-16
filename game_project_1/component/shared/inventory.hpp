#ifndef INVENTORY_H
#define INVENTORY_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/game/items.hpp>

//Inventory is private unless requested
class Inventory: public Component{
	public:

    int inventory_slots;
    ItemInstance* items;

    Inventory();
    ~Inventory();

    Component* Clone(ComponentParentContext context);
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};






#endif