#ifndef ITEM_H
#define ITEM_H

#include <game_project_1/io/serializer.hpp>

typedef int ItemID;

class ItemInstance{
    public:
    ItemID type;
    int stack_count;
    //int hone_level,hone_durability;

    ItemInstance();
    ~ItemInstance();
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

class ItemClass{
    ItemID type;
    wchar* name;
    //wchar* plural_name
    int slot;
    int max_stack;
    //WeaponInfo weapon;
    //ArmorInfo armor;

    //on_use;
    //on_swing;
    //on_take_damage;
    //on_pickup
    //on_remove
    //

};






#endif