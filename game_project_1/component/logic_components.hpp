#ifndef LOGIC_COMPONENTS_H
#define LOGIC_COMPONENTS_H

#include <game_project_1/io/serializer.hpp>
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
    
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Copy(StatBlock* s2);
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
    ~Equip();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Copy(Equip* i2);
};

//Inventory is private unless requested
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
    RaceID race_id;
    ClassID class_id;
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