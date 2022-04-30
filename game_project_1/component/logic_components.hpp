#ifndef LOGIC_COMPONENTS_H
#define LOGIC_COMPONENTS_H

#include <game_project_1/io/serializer.hpp>
#include <game_project_1/game/item.hpp>
#include <game_project_1/game/stats.hpp>


//RPG stat block
class StatBlock{
    public:
    int level;
    Stats base_stats;

    StatBlock();
    
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Copy(StatBlock* s2);
};

 //equipment and inventory. Only equipment is sent thru client delta, inventory is private unless requested (Difference between "Write" +"Save")
class Equip{
    public:
    ItemInstance head;
    ItemInstance body;
    ItemInstance gloves;
    ItemInstance boots;
    ItemInstance right_hand;
    ItemInstance left_hand;

    Equip();
    ~Equip();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Copy(Equip* i2);
};

class Inventory{
    public:
    int inventory_slots;
    ItemInstance* items;

    Inventory();
    ~Inventory();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Copy(Inventory* i2);
};

//Public character info: race, character class, appearance
class Character{
    public:
    int race_id;
    int class_id;
    CharacterAppearance appearance;

    Character();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Copy(Character* c2);
};


//A ServerEntity only component that assigns a globally unique ID
class Persistance{
    public:
    int global_id;//unique across all scenes+entities, corresponds to a SaveEntity
    Persistance();
};


#endif