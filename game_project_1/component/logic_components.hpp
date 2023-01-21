#ifndef LOGIC_COMPONENTS_H
#define LOGIC_COMPONENTS_H

#include <game_project_1/game/item.hpp>
#include <game_project_1/game/stats.hpp>
#include <game_project_1/game/races_and_classes.hpp>
#include <game_project_1/types/arrays.hpp>


//RPG stat block
class StatBlock{
    public:
    int level;
    BaseStats base_stats;

    StatBlock();
    StatBlock(int start_level);
    StatBlock(StatBlock* s2);
    ~StatBlock();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

 //Only equipment is sent thru client delta
class Equip{
    public:
    ItemInstance head;
    ItemInstance body;
    ItemInstance gloves;
    ItemInstance boots;
    ItemInstance right_hand;
    ItemInstance left_hand;

    Equip();
    Equip(Equip* s2);
    ~Equip();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

//Inventory is private unless requested
class Inventory{
    public:
    int inventory_slots;
    ItemInstance* items;

    Inventory();
    Inventory(Inventory* i2);
    ~Inventory();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};



//Public character info: race, character class, appearance
class Character{
    public:
    RaceID race_id;
    ClassID class_id;
    CharacterAppearance appearance;

    Character();
    Character(Character* c2);
    ~Character();
    
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};


//A ServerEntity only component that assigns a globally unique ID
class Persistance{
    public:
    int global_id;//unique across all scenes+entities, corresponds to a SaveEntity
    Persistance();
};

//Server-side NPC "brains"
typedef int NPCControllerType;
class NPCControllerState{
    public:
    NPCControllerType controller_type;
    bool init;//1-time-init control path flag that the controller callback can optionally use
    int current_action;
    int action_counter;
    
    Array<value> constants;
    Array<value> parameters;

    NPCControllerState(NPCControllerType type);
};


#endif